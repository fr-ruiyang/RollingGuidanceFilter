/***************************************************************/
/*
*   Distribution code Version 1.0 -- 08/14/2014 by Qi Zhang Copyright 2014, The Chinese University of Hong Kong.
*
*   The Code is created based on the method described in the following paper
*   [1] "Rolling Guidance Filter", Qi Zhang, Xiaoyong Shen, Li Xu, Jiaya Jia, European Conference on Computer Vision (ECCV) 2014
*
*   The code includes the bilateral filter implementation of:
*	[2] "Fast High-Dimensional Filtering Using the Permutohedral Lattice", Adams, Andrew, Jongmin Baek, and Myers Abraham Davis, Computer Graphics Forum. Vol. 29. No. 2. Blackwell Publishing Ltd, 2010.
*
*	PLEASE use the code in visual studio 2010/2012, Release Win32 Mode. (We only provide the openCV .lib/.dll files for this mode).
*
*   The code and the algorithm are for non-comercial use only.
*
/***************************************************************/

#ifndef RGF_H
#define RGF_H

/***************************************************************/
/*
 * Standard IO library is required.
 * STL String library is required.
 *
/***************************************************************/

#include <cstdio>
#include <string>

/***************************************************************/
/*
 * OpenCV 2.4 is required.
 * The following code is already tested on OpenCV 2.4.2.
 *
/***************************************************************/

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "permutohedral\macros.h"
#include "permutohedral\Image.h"
#include "permutohedral\permutohedral.h"
#include <time.h>

//Use the namespace of CV and STD
using namespace std;
using namespace cv;

class RollingGuidanceFilter{

public:

    /***************************************************************/
    /* Function: filter
     *
     * Description: rolling guidance filter core code by using bilateral filter.
     *
     * input arguments:
     *		  img: input image. Accept CV_8U and CV_32F type. Accept any number of channels.
     *    sigma_s: spatial sigma (default 3.0). Controlling the spatial weight of bilateral filter and also the filtering scale of rolling guidance filter.
     *    sigma_r: range sigma (default 0.1). Controlling the range weight of bilateral filter.
     *  iteration: the iteration number of rolling guidance (default 4).
     *
     */
    /***************************************************************/

    static Mat filter(Mat img, float sigma_s=3.0f, float sigma_r=0.1, int iteration=4){

        Mat I;

        //Change type to float
        img.convertTo(I,CV_MAKETYPE(CV_32F,img.channels()));

        Mat res = I.clone();

        // Filtering
        for(int i=0;i<iteration;i++){
            if(i)res = bilateralPermutohedral(I,res,sigma_s,sigma_r);
            else GaussianBlur(I,res,Size(0,0),sigma_s,sigma_s);
        }

        // Change type back
        res.convertTo(res,CV_MAKETYPE(img.depth(),img.channels()));

        return res;
    }

    /***************************************************************/
    /* Function: bilateralPermutohedral
     *
     * Description: bilateral filter by using permutohedral lattice. Invoking the bilateral filter implementation provided on http://graphics.stanford.edu/papers/permutohedral/.
     *
     * input arguments:
     *		  img: input image. Accept CV_32F type and any number of channels.
     *       edge: guidance image. Accept CV_32F type and any number of channels.
     *	  sigma_s: spatial sigma (default 3.0). Controlling the spatial weight of bilateral filter.
     *    sigma_r: range sigma (default 0.1). Controlling the range weight of bilateral filter.
     *
     */
    /***************************************************************/

    static Mat bilateralPermutohedral(Mat img, Mat edge, float sigma_s, float sigma_r){ // img: float [0,1]

        float invSpatialStdev = 1.0f/sigma_s;
        float invColorStdev = 1.0f/sigma_r;

        // Construct the position vectors out of x, y, r, g, and b.
        int height = img.rows;
        int width = img.cols;

        int eCh = edge.channels();
        int iCh = img.channels();
        Image positions(1, width, height, 2+eCh);
        Image input(1, width, height, iCh);

        //From Mat to Image
        for (int y = 0; y < height; y++) {
            float *pimg = img.ptr<float>(y);
            float *pedge = edge.ptr<float>(y);
            for (int x = 0; x < width; x++) {
                positions(x, y)[0] = invSpatialStdev * x;
                positions(x, y)[1] = invSpatialStdev * y;

                for(int c=0; c<eCh; c++)
                    positions(x, y)[2+c] = invColorStdev * pedge[x*eCh+c];

                for(int c=0; c<iCh; c++)
                    input(x, y)[c] = pimg[x*iCh+c];
            }
        }

        // Filter the input with respect to the position vectors. (see permutohedral.h)
        Image out = PermutohedralLattice::filter(input, positions);

        // Save the result
        Mat imgOut(img.size(), img.type());
        for (int y = 0; y < height; y++) {
            float *pimgOut = imgOut.ptr<float>(y);
            for (int x = 0; x < width; x++) {
                for(int c=0; c<iCh; c++)
                    pimgOut[x*iCh+c] = out(x, y)[c];
            }
        }

        return imgOut;
    }
};

#endif
