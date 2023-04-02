import numpy as np
import matplotlib.pyplot as plt

def get_xn(Xs,n):
    '''
    calculate the Fourier coefficient X_n of 
    Discrete Fourier Transform (DFT)
    '''
    L  = len(Xs)
    ks = np.arange(0,L,1)
    xn = np.sum(Xs*np.exp((1j*2*np.pi*ks*n)/L))/L
    return(xn)

def get_xns(ts):
    '''
    Compute Fourier coefficients only up to the Nyquest Limit Xn, n=1,...,L/2
    and multiply the absolute value of the Fourier coefficients by 2, 
    to account for the symetry of the Fourier coefficients above the Nyquest Limit. 
    '''
    mag = []
    L = len(ts)
    for n in range(int(L/2)): # Nyquest Limit
        mag.append(np.abs(get_xn(ts,n))*2)
    return(mag)

def get_Hz_scale_vec(ks,sample_rate,Npoints):
    freq_Hz = ks*sample_rate/Npoints
    freq_Hz  = [int(i) for i in freq_Hz ] 
    return(freq_Hz )

def create_spectrogram(ts,NFFT,n_mels,noverlap = None):
    '''
          ts: original time series
        NFFT: The number of data points used in each block for the DFT.
          Fs: the number of points sampled per second, so called sample_rate
    noverlap: The number of points of overlap between blocks. 
    '''
    if noverlap is None:
        noverlap = NFFT/2
    noverlap = int(noverlap)
    starts  = np.arange(0,len(ts),NFFT-noverlap,dtype=int)
    starts  = starts[starts + NFFT < len(ts)]
    len_nfft_nmels = (NFFT//2)//n_mels#-1
    xns = []
    for start in starts:        
#         ts_window = get_xns(ts[start:start + NFFT]) 
        ts_window = np.abs(np.fft.fft(ts[start:start+NFFT]))
#         ts_window = np.abs(np.real(np.fft.fft(ts[start:start+NFFT])))
        
        ts_window_n_mel = []
        for i in range(n_mels):
            promedio = ts_window[i*len_nfft_nmels:i*len_nfft_nmels+len_nfft_nmels].mean()
#             suma = 0
#             for j in range(len_nfft_nmels):
#                 suma += ts_window[i*len_nfft_nmels+j]
#             promedio = suma/len_nfft_nmels
            ts_window_n_mel.append(promedio)
        xns.append(ts_window_n_mel)

    spec = np.array(xns)
    #spec = 10*np.log10(spec)
    assert spec.shape[0] == len(starts) 
    return(starts,spec)

def plot_spectrogram(spec,ks,sample_rate, L, starts, mappable = None):
    plt.figure(figsize=(20,8))
    plt_spec = plt.imshow(spec,origin='lower')
    plt.title("Spectrogram L={} Spectrogram.shape={}".format(L,spec.shape))
    plt.colorbar(mappable,use_gridspec=True)
    plt.show()
    return(plt_spec)