#include <stdio.h>

#include "write.h"
#include "messages.h"
#include "modelParameters.h"
#include "State.h"
#include "icMatrix.h"
#include "thermalLuminosities.h"
#include "globalVariables.h"

#include "targetField.h"
#include "thermalDistribution.h"
#include "radiativeLosses.h"
#include "injection.h"
#include "distribution.h"
#include "processes.h"


#include <fparameters/parameters.h>
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

		if (calculateScatt) {
			show_message(msgStart, Module_torusSampling);
			icMatrix(model);
			show_message(msgEnd, Module_torusSampling);
		} else {
			icMatrixRead(model);
		}

		thermalLuminosities(model,"lum.txt");
		
//***********nonthermal particles**************		
		
		//completo las distribuciones de los termicos con Maxwell-Jutner
		//thermalDistribution(model.electron, model); 
		//thermalDistribution(model.proton, model);
		//targetField(model);
		
		//radiativeLosses(model.ntElectron, model, "electronLosses.txt");
		//radiativeLosses(model.ntProton, model, "protonLosses.txt");
		
		//nt electrons
		injection(model.ntElectron, model);
		writeEandRParamSpace("electronInj",model.ntElectron.injection,0);
		distribution2(model.ntElectron, model);
		writeEandRParamSpace("electronDis",model.ntElectron.distribution,0);
		
		//nt protons
		//injection(model.ntProton,model);
		//writeEandRParamSpace("protonInj", model.ntProton.injection, 0);
		//distribution2(model.ntProton,model);
		//writeEandRParamSpace("protonDis", model.ntProton.distribution, 0);

		processes(model, "ntLuminosities.txt");
		
//********************************************

	}
	catch (std::runtime_error& e)
	{
		std::cout << "ERROR: " << e.what() << std::endl;
	}
	return 0;
}