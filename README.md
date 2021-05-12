# SLAM Project: Dynamic Object Removal With ORB-SLAM2
**Group members:** Di Deng, Radhika Mohanan, and Samiran Gode
### Prerequisites
We have tested the code with Ubuntu 18.04.
### Simulation Environment
- The simulation environment is obtained from UnrealEngine MarketPlace [Modular
 School Pack](https://www.unrealengine.com/marketplace/en-US/product/modular-school-pack)
- The walking character, Brian, is downloaded from Adobe [mixamo](https://www.mixamo.com/#/).
<div align="center">
    <a href="https://github.com/AndieDeng/dynamic_feature_removal_ORBSLAM2">
      <img src="videos/1dataset.gif" alt="simulation environment">
   </a>
</div>

### Dynamic Object Extraction from Semantic Images
<div align="center">
  <a href="https://github.com/AndieDeng/dynamic_feature_removal_ORBSLAM2">
    <img src="videos/2get_dynamic_obstacles.gif" alt="dynamic obstacle
     extraction">
  </a>
</div>

### Feature Extraction with Dynamic Oject Removal and Pur ORB Feature Extractor
<div align="center">
  <a href="https://github.com/AndieDeng/dynamic_feature_removal_ORBSLAM2">
    <img src="videos/3orb_slam2_result.gif" alt="ORB feature extractor">
  </a>
</div>

<div align="center">
  <a href="https://github.com/AndieDeng/dynamic_feature_removal_ORBSLAM2">
    <img src="videos/4dynamic_feature_removal_result.gif" alt="remove
     features on dynamic objects">
  </a>
</div>

<p align="left">
  <img src="images/1.png" width=350>
  <img src="images/2.png" width=350>
</p>

<p align="left">
  <img src="images/3.png" width=350>
  <img src="images/4.png" width=350>
</p>

<p align="left">
  <img src="images/5.png" width=350>
  <img src="images/6.png" width=350>
</p>

### Execution Steps
1. Install [ORB-SLAM2](https://github.com/raulmur/ORB_SLAM2)
2. Replace **src** and **include** folders with the files in this repo
3. Run
```
chmod +x build.sh
./build.sh
./Examples/Monocular/mono_dynamic_airsim Vocabulary/ORBvoc.txt Examples/Monocular/airsim.yaml dynamic_obstacle_dataset
```
You can find that all features on dynamic objects are removed using this code
 comparing to the features extracted directly using ORB-SLAM2.