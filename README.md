# BionicArmControl
So this repository is for the control team division of the Arm team. 

This is divided into Time-Delay Neural Network (TDNN) code, and EMG Direct Control currently.

**TDNN**

- 	This is a concept we came up with to interpret an EMG signal using a TDNN (read more here: https://kaleidoescape.github.io/tdnn/) in order to create an offline model, and then use classification machine learning on the online to be used in real-time. This is inline with paper's such as: https://ieeexplore.ieee.org/abstract/document/8868796
	
	Currently, we are trying to implement this model via using MATLAB's timedelaynet function and deep learning toolbox.
	
		
		
**Direct Control**
	
-	This is a concept involving using the EMG signal directly to control the arm. This has downsides of lower DOF, but is more simplistic in nature
	
**Terms Used:**
	Offline: This is computations done previously on a powerful computer which is then uploaded to the arm.
	Online: This is what the arm uses in real-time
	DOF: This is the number of axes of movement the arm has.

	
