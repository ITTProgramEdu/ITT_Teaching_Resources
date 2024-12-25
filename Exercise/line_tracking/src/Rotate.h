#ifndef __ROTATE__
#define __ROTATE__

void Rotate_Go(unsigned int inspect12, double normal);
bool Rotate_Finish(unsigned int inspect12,double normal);
bool Rotate_Adjust(double current_angle,unsigned int inspect12);
void Stop_Rotate();
bool Turn(unsigned int inspect12,double normal);

bool Find_Parallel(unsigned int inspect12,double normal);

#endif