//
// Created by dideng on 5/11/21.
//

#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>

#include<opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/videoio.hpp>
#include<System.h>

using namespace std;

void LoadImages(const string &strFile, vector<string> &vstrImageFilenames,
                vector<double> &vTimestamps);

int main(int argc, char **argv)
{
  if(argc != 4)
  {
    cerr << endl << "Usage: ./mono_tum path_to_vocabulary path_to_settings path_to_sequence" << endl;
    return 1;
  }

  // Retrieve paths to images
  vector<string> vstrRgbImageFilenames, vstrSemanticImageFilenames;
  vector<double> vTimestamps;
  string strRgbFile = string(argv[3])+"/rgb.txt";
  string strSemanticFile = string(argv[3])+"/semantic.txt";
  LoadImages(strRgbFile, vstrRgbImageFilenames, vTimestamps);
  LoadImages(strSemanticFile, vstrSemanticImageFilenames, vTimestamps);
  int nImages = vstrRgbImageFilenames.size();

  // Create SLAM system. It initializes all system threads and gets ready to process frames.
  ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::MONOCULAR,true);

  // Vector for tracking time statistics
  vector<float> vTimesTrack;
  vTimesTrack.resize(nImages);

  cout << endl << "-------" << endl;
  cout << "Start processing sequence ..." << endl;
  cout << "Images in the sequence: " << nImages << endl << endl;

  // Main loop
  cv::Mat im_rgb, im_semantic, output;
  cv::Size frame_size(640, 480);
//  cv::VideoWriter video_writer("semantic.avi",
//                               cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
//                               30, frame_size, true);
  for(int ni=0; ni<nImages; ni++)
  {
    // Read image from file
    im_rgb = cv::imread(string(argv[3])+"/"+vstrRgbImageFilenames[ni], CV_LOAD_IMAGE_UNCHANGED);
    im_semantic = cv::imread(string(argv[3])+"/"+vstrSemanticImageFilenames[ni], CV_LOAD_IMAGE_UNCHANGED);
    double tframe = vTimestamps[ni];

    if(im_rgb.empty() || im_semantic.empty())
    {
      cerr << endl << "Failed to load image at: "
           << string(argv[3]) << "/" << vstrRgbImageFilenames[ni] << "or at: "
           << string(argv[3]) << "/" << vstrSemanticImageFilenames[ni] << endl;
      return 1;
    }
    cv::inRange(im_semantic, cv::Scalar(0, 130, 70), cv::Scalar(30, 140, 75),output);
//    video_writer.write(im_semantic);

//    cv::imshow( "semantic", im_semantic);
//    cv::imshow( "filtered", output);

#ifdef COMPILEDWITHC11
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
#else
    std::chrono::monotonic_clock::time_point t1 = std::chrono::monotonic_clock::now();
#endif

    // Pass the image to the SLAM system
    SLAM.TrackMonocular(im_rgb, output, tframe);

#ifdef COMPILEDWITHC11
    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
#else
    std::chrono::monotonic_clock::time_point t2 = std::chrono::monotonic_clock::now();
#endif
    double ttrack= std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1).count();

    vTimesTrack[ni]=ttrack;

    // Wait to load the next frame
    double T=0;
    if(ni<nImages-1)
      T = vTimestamps[ni+1]-tframe;
    else if(ni>0)
      T = tframe-vTimestamps[ni-1];

    if(ttrack<T)
      usleep((T-ttrack)*1e6);
//    cv::waitKey(33);
  }
//  video_writer.release();
  // Stop all threads
  SLAM.Shutdown();

  // Tracking time statistics
  sort(vTimesTrack.begin(),vTimesTrack.end());
  float totaltime = 0;
  for(int ni=0; ni<nImages; ni++)
  {
    totaltime+=vTimesTrack[ni];
  }
  cout << "-------" << endl << endl;
  cout << "median tracking time: " << vTimesTrack[nImages/2] << endl;
  cout << "mean tracking time: " << totaltime/nImages << endl;

  // Save camera trajectory
  SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");
  return 0;
}

void LoadImages(const string &strFile, vector<string> &vstrImageFilenames, vector<double> &vTimestamps)
{
  ifstream f;
  f.open(strFile.c_str());

  // skip first three lines
  string s0;
  getline(f,s0);
  getline(f,s0);
  getline(f,s0);

  while(!f.eof())
  {
    string s;
    getline(f,s);
    if(!s.empty())
    {
      stringstream ss;
      ss << s;
      double t;
      string sRGB;
      ss >> t;
      vTimestamps.push_back(t);
      ss >> sRGB;
      vstrImageFilenames.push_back(sRGB);
    }
  }
}
