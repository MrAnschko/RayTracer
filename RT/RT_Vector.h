#ifndef RT_VECTOR_H
#define RT_VECTOR_H
#include <vector>


class RT_Vector
{
    public:
        float data[3];
        RT_Vector(float x, float y, float z);
        RT_Vector();
        ~RT_Vector();
        void Print();
        RT_Vector Copy();
        RT_Vector Normalize(); // Returns a normalized vector (according to p2 norm); or null if normalization is not possible. Not In place so, the original vector will remain the same.
        RT_Vector Negate(); //returns NEW vector that is the additive inverse;
        float Length(); // Returns the length of the vector (according to p2-norm/euclidian);
        static RT_Vector CrossProduct(RT_Vector vec_a, RT_Vector vec_b); // returns the vector that is the result of the cross product of vector a and vector b;
        static float DotProduct(RT_Vector vec_a, RT_Vector vec_b); // returns the vector that is the result of the cross product of vector a and vector b;
        static RT_Vector HadamardProduct(RT_Vector vec_a, RT_Vector vec_b); // returns the vector that is the result of the cross product of vector a and vector b;
        static RT_Vector Add(RT_Vector vec_a, RT_Vector vec_b); //  returns the vector that is the result of the cross product of vector a and b
        RT_Vector operator + (RT_Vector v);
        RT_Vector operator - (RT_Vector v);
        static RT_Vector ScalarProduct(float scalar, RT_Vector vec); //  returns the vector that is the result of the scalar product of a vector and a scalar
        static bool Equals(RT_Vector vec_a, RT_Vector vec_b); //  returns true if both vectors have the same values
        
};

#endif
