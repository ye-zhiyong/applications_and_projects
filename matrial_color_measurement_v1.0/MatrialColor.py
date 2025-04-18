from multiprocessing.spawn import get_preparation_data
from pyexpat import model
from xml.etree.ElementTree import tostring
import baseline
from matplotlib.lines import lineStyles
from more_itertools import combination_with_replacement_index
from networkx import dfs_edges
import pandas as pd
import numpy as np
import colour
from colour import MSDS_CMFS, SDS_ILLUMINANTS
import matplotlib.pyplot as plt
from scipy  import interpolate
from scipy.signal import savgol_filter
from sklearn import preprocessing
from pybaselines import Baseline

class MatrialColor:
    def __init__(self, spectrum_file_path):
        self.spectrum_file_path = spectrum_file_path

    def get_spectrogram(self, df_spectrum):
        i = 0
        df_spectrum = df_spectrum * 100
        df_spectrum['wavelength'] = df_spectrum['wavelength']  * 0.01
        plt.figure(figsize=(10, 6))
        for col in df_spectrum.columns[1:6]:
            plt.plot(df_spectrum['wavelength'], df_spectrum[col], label = col, linestyle = '-')
        plt.xlabel('Wavelength(nm)')
        plt.ylabel('Reflectance(%)')
        plt.title('Reflectance')
        plt.grid(alpha = 0)
        plt.legend()
        plt.show()

    def get_preprocess_spectrum_data(self):
        df = pd.read_excel(self.spectrum_file_path, header = 0)
        # get intensity's average (multisampling)
        df_spectrum = pd.DataFrame()
        df_spectrum['wavelength'] = df['wavelength']
        df_spectrum['raw_intensity'] = df['raw_intensity']
        for i in [1, 2, 3, 4, 5]:
            cols = [col for col in df.columns if col.startswith(f'intensity_sample_{i}')]
            df_spectrum[f'intensity_sample_{i}'] = df[cols].mean(axis = 1)
        # interval = 1nm
        df_preprocessed = pd.DataFrame()
        df_preprocessed['wavelength'] = np.arange(df_spectrum['wavelength'].min(), df_spectrum['wavelength'].max(), 1.0)
        for col in df_spectrum.columns[1:]:
            # linear interpolation
            f = interpolate.interp1d(df_spectrum['wavelength'], df_spectrum[col], kind = 'linear') 
            df_preprocessed[col] = f(df_preprocessed['wavelength'])
            # filter
            #df_preprocessed[col] = savgol_filter(df_preprocessed[col], window_length = 7, polyorder = 3, mode = 'nearest')
            # baseline correction
            baseline_filter = Baseline(x_data = df_preprocessed['wavelength'])
            df_preprocessed[col], params = baseline_filter.asls(df_preprocessed[col], lam=1e2, p=0.99, max_iter=30)
        print(df_preprocessed)
        return df_preprocessed

    def get_reflectance(self):
        df_spectrum  = self.get_preprocess_spectrum_data()
        df_reflc = pd.DataFrame()
        df_reflc['wavelength'] = df_spectrum['wavelength']
        #for col in df_raw.columns:
            #if col.startswith('test_intensity'):
                #i += 1
                #df_reflc[f'reflectance_{i}'] =  df_raw[col] / df_raw['raw_intensity'] 
        i = 0
        for col in [col for col in df_spectrum.columns if col.startswith('intensity_sample')]:
                i += 1
                # calculate reflectance based reference board reflectance
                reflectance_reference_board = 1
                df_reflc[f'reflectance_{i}'] =  df_spectrum[col] / df_spectrum['raw_intensity'] * reflectance_reference_board
                # compensation correction
                #offset_compensation = 0.01
                #df_reflc[f'reflectance_{i}'] +=  offset_compensation
        df_reflc = df_reflc[(df_reflc['wavelength'] >= 400) & (df_reflc['wavelength'] <= 780)] 
        df_reflc['wavelength'] = df_reflc['wavelength'] 
        print(df_reflc)
        return df_reflc

    def get_XYZ(self, illuminant):
        # load reflectance data
        #df_reflc = pd.read_excel(self.spectrum_file_path, header = 0)
        #df_reflc = df_reflc[(df_reflc['wavelength'] >= 380) & (df_reflc['wavelength'] <= 780)]
        df_reflc = self.get_reflectance()
        # load CIE standard and illuminant
        cmfs = colour.MSDS_CMFS["CIE 1931 2 Degree Standard Observer"]
        illum = colour.SDS_ILLUMINANTS[illuminant]
        #illum = illum.copy().trim(colour.SpectralShape(380, 780, 1))
        # calculate XYZ
        df_XYZ = pd.DataFrame()
        i = 0
        for col in [col for col in df_reflc.columns if col.startswith('reflectance')]:
            sd = colour.SpectralDistribution(df_reflc[col], df_reflc['wavelength'])
            #sd = sd.copy().trim(colour.SpectralShape(380, 780, 1))
            XYZ = colour.sd_to_XYZ(sd, cmfs, illum)
            # XYZ normalization
            i += 1
            XYZ_n = np.array([95.047, 100.0, 108.883])  # D65 白点
            df_XYZ[f'xyz_{i}'] = XYZ / XYZ_n
        print(df_XYZ)
        return df_XYZ
    
    def get_LAB(self, illuminant):
        df_XYZ = self.get_XYZ(illuminant)
        df_LAB = pd.DataFrame()
        delta = 6 / 29
        f = lambda t: np.where(t > delta**3, t**(1/3), t/(3*delta**2) + 4/29)
        i = 0
        for sample in df_XYZ.columns:
            i += 1    
            L = 116 * f(df_XYZ[sample].iloc[1]) - 16
            A = 500 * (f(df_XYZ[sample].iloc[0]) - f(df_XYZ[sample].iloc[1]))
            B = 200 * (f(df_XYZ[sample].iloc[1]) - f(df_XYZ[sample].iloc[2]))
            df_LAB[f'lab_{i}'] = [L, A, B]
        print(df_LAB)
        df_LAB.to_excel('spectrum_20250416/lab_data_2.xlsx', index = False)
        return df_LAB

        
    
if __name__ == "__main__":

    matrial_color = MatrialColor("spectrum_20250416/spectrum_data.xlsx")
    #df_spectrum = matrial_color.get_preprocess_spectrum_data()
    df_rec = matrial_color.get_reflectance()
    #df_rec = pd.read_excel('spectrum_customer/1-10.xlsx')
    #df_rec = df_rec[(df_rec['wavelength'] >= 400) & (df_rec['wavelength'] <= 780)]
    matrial_color.get_spectrogram(df_rec)
    #matrial_color.get_reflectance()
    matrial_color.get_LAB("D65")


