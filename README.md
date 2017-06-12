# histogram
'histExample' presents source code for measuring histogram from images and plot them in cv Window!

Helpful links:
http://www.aishack.in/tutorials/drawing-histograms-opencv/

http://karthicknopencv.blogspot.com/2010/04/getting-histogram-for-given-grayscale.html

http://www.programming-techniques.com/2013/01/histogram-equalization-using-c-image.html

'compHistExmp' presents source code for comparing two histogram based on correlation function, CHI-SQUARE algorithm, interscetion and Bhattacharyya distance. All thses have been done using openCV lib. OpenCV histogram comparison link is given below:
http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_comparison/histogram_comparison.html


Some Important info about Chi-Square algorithm 
https://www.researchgate.net/post/What_is_chi-squared_distance_I_need_help_with_the_source_code

https://stats.stackexchange.com/questions/184101/comparing-two-histograms-using-chi-square-distance

http://www.itl.nist.gov/div898/handbook/eda/section3/eda35f.htm


'compHistExmp2' presents the similar histogram comparison program as same as 'compHistExmp'. The main difference is that all the functions to excute the algorithms have been made from scratch. No openCV lib has been used. The following functions are stated in the 'compHistExmp2' file...

imhist -> for measuring histogram 

m2_chiSquare -> measuring quadric chi square value
detail of quadric chi square can be found here:
https://stats.stackexchange.com/questions/184101/comparing-two-histograms-using-chi-square-distance

m1_chiSquare -> measuring chi-square value (regular) 
for detail:
https://stats.stackexchange.com/questions/184101/comparing-two-histograms-using-chi-square-distance

normHistUsingTrapz -> Normalization using Trapezoidal numerical integration
for details:
https://stackoverflow.com/questions/5320677/how-to-normalize-a-histogram-in-matlab
Trapezoidal: https://www.mathworks.com/help/matlab/ref/trapz.html

normHistUsingPdf -> Normalization using PDF
for details:
https://stackoverflow.com/questions/5320677/how-to-normalize-a-histogram-in-matlab

intersection-> Measure intersection
correlation-> measure correlation
bhattacharyyaDis->Measure Bhattacharyya distance
For details of these three:
http://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_comparison/histogram_comparison.html






