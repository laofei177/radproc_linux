import numpy as np
import matplotlib.pyplot as plt
import matplotlib

from matplotlib import rc        # TO MANAGE MATPLOTLIB PARAMETERS"
rc('font',family='serif')
rc('text',usetex = True)

import scipy.optimize as optimization

nu,eV,Sy,Br,IC,pp,CD,Refl,Tot = np.loadtxt('lum.txt',unpack=True)
NT_logeV,NT_logSye,NT_logSyp,NT_logIC,NT_logpp,NT_logpg,NT_logeAbs,NT_logpAbs = \
        np.loadtxt('ntLuminosities.txt',unpack=True,skiprows=1)
NT_logeVpairs, NT_logSypairs, NT_logICpairs, NT_logAbsPairs = \
        np.loadtxt('ntPairtLum.txt', unpack=True, skiprows=1)
x_eV = [-9,19]
y_axis = [33,43]

fig, ax1 = plt.subplots()

ax1.tick_params(axis='both',labelsize=12)
ax1.set_xlim(x_eV)
ax1.set_ylim(y_axis)

ax1.set_xlabel(r'$\mathrm{Log}(E/\mathrm{eV})$',fontsize=13)
ax1.set_ylabel(r'$\mathrm{Log}(\nu L_\nu / \mathrm{erg~s}^{-1})$',fontsize=13)

ax1.plot(np.log10(eV),np.log10(Tot),label='Thermal')
ax1.plot(NT_logeV,NT_logSye,label='eSy')
ax1.plot(NT_logeV,NT_logSyp,label='pSy')
ax1.plot(NT_logeV,NT_logIC,label='IC')
ax1.plot(NT_logeV,NT_logpp,label='pp')
ax1.plot(NT_logeV,NT_logpg,label=r'p$\gamma$')
ax1.plot(NT_logeVpairs,NT_logSypairs,label='seSy')
ax1.plot(NT_logeVpairs,NT_logICpairs,label='seIC')
ax1.plot(NT_logeV,np.log10(np.power(10,NT_logeAbs)+np.power(10,NT_logpAbs)+Tot+np.power(10,NT_logAbsPairs)),\
         lw=3,label='Abs',color='k')
ax1.legend(loc='best',fontsize=8)

fig.savefig('nonThermalLum.pdf')