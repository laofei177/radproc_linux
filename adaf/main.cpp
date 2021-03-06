#include <stdio.h>
#include "write.h"
#include "read.h"
#include "messages.h"
#include "modelParameters.h"
#include "State.h"
#include "comptonScattMatrix.h"
#include "thermalProcesses.h"
#include "adafFunctions.h"
#include "globalVariables.h"
#include "flareProcesses.h"
#include "redshiftFunction.h"

#include "thermalDistribution.h"
#include "NTtimescales.h"
#include "NTinjection.h"
#include "injectionNeutrons.h"
#include "distributionNeutrons.h"
#include "NTdistribution.h"
#include "NTradiation.h"
#include "absorption.h"
#include "secondariesProcesses.h"
#include "jetEmission.h"

#include <fparameters/parameters.h>
#include <fparameters/SpaceIterator.h>
#include <inout/ioutil.h>
#include <iostream>
#include <boost/property_tree/ptree.hpp>

using namespace std;
int main()
{
	string folder{prepareOutputfolder()};
	try {
		GlobalConfig = readConfig();
		prepareGlobalCfg();
		
		show_message(msgStart, Module_state);
		State model(GlobalConfig.get_child("model"));
		show_message(msgEnd, Module_state);

		writeFields(model);

		if (calculateComptonScatt) {
			show_message(msgStart,Module_comptonScattMatrix);
			comptonScattMatrix(model);
			show_message(msgEnd,Module_comptonScattMatrix);
		} else {
			comptonScattMatrixRead(model);
		}
		
		redshiftFactor(model);
		
		if (calculateThermal) {
			thermalRadiation(model,"lumThermal.dat");
			writeEandRParamSpace("photonDensity",model.photon.distribution,0,0);
			writeRParamSpace("photonDensity_R",model.photon.distribution,0,0);
		} else {
			readEandRParamSpace("photonDensity",model.photon.distribution,0,0);
		}
		
		writeEandRParamSpace("photonDensity_z",model.photon.injection,0,0);
		
		if (calculateJetEmission)
			jetProcesses(model,"lumJet.txt");
		
		if (calculateFlare)
			flareProcesses(model);
		
//***********nonthermal particles**************		
		if (calculateNonThermal) {
            
			if (calculateLosses) {
				show_message(msgStart, Module_radLosses);
				if (calculateNTelectrons)
					nonThermalTimescales(model.ntElectron, model, "electronCoolingTimes.dat");
				if (calculateNTprotons)
					nonThermalTimescales(model.ntProton, model, "protonCoolingTimes.dat");
				show_message(msgEnd, Module_radLosses);
			}
		
			if (calculateNTdistributions) {

				if (calculateNTelectrons) {
					injection(model.ntElectron, model);
					writeEandRParamSpace("electronInjection_vol",model.ntElectron.injection,0,1);
					writeEandRParamSpace("electronInjection",model.ntElectron.injection,0,0);
					writeRParamSpace("electronInjection_R", model.ntElectron.injection, 0, 0);
					if (accMethod == 0)
						//distributionSpatialDiffusion(model.ntElectron, model);
						distributionMultiZone(model.ntElectron, model);
						//distributionSpatialDiffusionSteady(model.ntElectron, model);
					else
						distributionSpatialDiffusion(model.ntElectron, model);
					writeEandRParamSpace("electronDistribution_vol",model.ntElectron.distribution,0,1);
					writeEandRParamSpace("electronDistribution",model.ntElectron.distribution,0,0);
					writeRParamSpace("electronDistribution_R", model.ntElectron.distribution, 0, 0);
				} else {
					readEandRParamSpace("electronInjection_vol",model.ntElectron.injection,0,1);
					readEandRParamSpace("electronDistribution_vol",model.ntElectron.distribution,0,1);
				}
				
				if (calculateNTprotons) {
					if (accMethod == 0)
						injection(model.ntProton,model);
					else
						injection(model.ntProton,model);
						//injectionFokkerPlanckOneZone(model.ntProton,model);
					writeEandRParamSpace("protonInjection_vol", model.ntProton.injection, 0, 1);
					writeEandRParamSpace("protonInjection", model.ntProton.injection, 0, 0);
					writeRParamSpace("protonInjection_R", model.ntProton.injection, 0, 0);
					if (accMethod == 0)
						distributionSpatialDiffusionSteady(model.ntProton, model);
						//distributionMultiZoneRadial(model.ntProton, model);
					else
						distributionSpatialDiffusionSteady(model.ntProton, model);
						//distributionFokkerPlanckCompleteSteadyState(model.ntProton, model);
						//distributionFokkerPlanckSpatialDiffusionTimeDependent(model.ntProton, model);
						//distributionFokkerPlanckRadial(model.ntProton,model);
					writeEandRParamSpace("protonDistribution_vol", model.ntProton.distribution,0,1);
					writeEandRParamSpace("protonDistribution", model.ntProton.distribution,0,0);
					writeRParamSpace("protonDistribution_R", model.ntProton.distribution, 0,0);
				} else {
					readEandRParamSpace("protonInjection_vol", model.ntProton.injection,0,1);
					readEandRParamSpace("protonDistribution_vol", model.ntProton.distribution,0,1);
				}
			} else {
				if (calculateNTelectrons) {
					readEandRParamSpace("electronInjection_vol",model.ntElectron.injection,0,1);
					readEandRParamSpace("electronDistribution_vol",model.ntElectron.distribution,0,1);
				}
				if (calculateNTprotons) {
					readEandRParamSpace("protonInjection_vol",model.ntProton.injection,0,1);
					readEandRParamSpace("protonDistribution_vol",model.ntProton.distribution,0,1);
				}
			}
			
			if (calculateNonThermalLum) {
				nonThermalRadiation(model,"lumNonThermal.dat");
				writeEandRParamSpace("photonDensity",model.photon.distribution,0,0);
				writeRParamSpace("photonDensity_R",model.photon.distribution,0,0);
				writeEandRParamSpace("NTphotonDensity",model.ntPhoton.distribution,0,0);
				writeRParamSpace("NTphotonDensity_R",model.ntPhoton.distribution,0,0);
				writeEandRParamSpace("opticalDepth_gg",model.tau_gg,0,0);
			} else {
				readEandRParamSpace("photonDensity",model.photon.distribution,0,0);
				readEandRParamSpace("NTphotonDensity",model.ntPhoton.distribution,0,0);
				readEandRParamSpace("NTphotonDensity",model.ntPhoton.injection,0,0);
				readEandRParamSpace("opticalDepth_gg",model.tau_gg,0,0);
			}
			
			if (calculateSecondaries) {
				secondariesProcesses(model);
				writeEandRParamSpace("NTphotonDensity",model.ntPhoton.distribution,0,0);
			} else {
				readEandRParamSpace("muonDistribution",model.ntMuon.distribution,0,0);
				readEandRParamSpace("pionDistribution",model.ntChargedPion.distribution,0,0);
			}
			
			// NEUTRINO TRANSPORT
			if (calculateNeutrinos)
				injectionNeutrino(model.neutrino,model);
				
			// NEUTRON TRANPOSRT
			if (calculateNeutronInj) {
				injectionNeutrons(model);
				radiativeLossesNeutron(model.ntNeutron,model,"neutronTimescales.dat");
				if (calculateNeutronDis)
					distributionNeutronsAGN(model);
				if (calculateJetDecay)
					jetNeutronDecay(model);
			}
		}
	}
	catch (std::runtime_error& e)
	{
		std::cout << "ERROR: " << e.what() << std::endl;
	}
	return 0;
}




/* prueba interpol

		model.ntElectron.ps.iterate([&](const SpaceIterator& i) {
		
			double E = i.val(0);
			double u = model.ntElectron.emin()*10.0;
			
			double Qe = model.ntElectron.injection.interpolate({ { 0, u } }, &i.coord); 
			
			double res = Qe;
			
			
		});*/