#!/bin/bash
pathDatasetEuroc='/home/shared' #Example, it is necesary to change it by the dataset path

#------------------------------------
# Monocular-Inertial Examples
echo "Launching MH01 with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/EuRoC.yaml "$pathDatasetEuroc"/MH01 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/MH01.txt dataset-MH01_monoi
exit
echo "Launching MH02 with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/EuRoC.yaml "$pathDatasetEuroc"/MH02 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/MH02.txt dataset-MH02_monoi

echo "Launching MH03 with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/EuRoC.yaml "$pathDatasetEuroc"/MH03 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/MH03.txt dataset-MH03_monoi

echo "Launching MH04 with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/EuRoC.yaml "$pathDatasetEuroc"/MH04 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/MH04.txt dataset-MH04_monoi

echo "Launching MH05 with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/EuRoC.yaml "$pathDatasetEuroc"/MH05 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/MH05.txt dataset-MH05_monoi

echo "Launching V101 with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/EuRoC.yaml "$pathDatasetEuroc"/V101 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/V101.txt dataset-V101_monoi

echo "Launching V102 with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/EuRoC.yaml "$pathDatasetEuroc"/V102 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/V102.txt dataset-V102_monoi

echo "Launching V103 with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/EuRoC.yaml "$pathDatasetEuroc"/V103 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/V103.txt dataset-V103_monoi

echo "Launching V201 with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/EuRoC.yaml "$pathDatasetEuroc"/V201 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/V201.txt dataset-V201_monoi

echo "Launching V202 with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/EuRoC.yaml "$pathDatasetEuroc"/V202 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/V202.txt dataset-V202_monoi

echo "Launching V203 with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/EuRoC.yaml "$pathDatasetEuroc"/V203 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/V203.txt dataset-V203_monoi

# MultiSession Monocular Examples
echo "Launching Machine Hall with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/EuRoC.yaml "$pathDatasetEuroc"/MH01 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/MH01.txt "$pathDatasetEuroc"/MH02 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/MH02.txt "$pathDatasetEuroc"/MH03 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/MH03.txt "$pathDatasetEuroc"/MH04 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/MH04.txt "$pathDatasetEuroc"/MH05 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/MH05.txt dataset-MH01_to_MH05_monoi

echo "Launching Vicon Room 1 with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/EuRoC.yaml "$pathDatasetEuroc"/V101 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/V101.txt "$pathDatasetEuroc"/V102 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/V102.txt "$pathDatasetEuroc"/V103 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/V103.txt dataset-V101_to_V103_monoi

echo "Launching Vicon Room 2 with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc ./Vocabulary/ORBvoc.txt ./Examples/Monocular-Inertial/EuRoC.yaml "$pathDatasetEuroc"/V201 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/V201.txt "$pathDatasetEuroc"/V202 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/V202.txt "$pathDatasetEuroc"/V203 ./Examples/Monocular-Inertial/EuRoC_TimeStamps/V203.txt dataset-V201_to_V203_monoi
