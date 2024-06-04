//
// Created by c2-horosnyi on 25/02/2020.
//

#ifndef ESDLB3_CONTACT_H
#define ESDLB3_CONTACT_H


class Contact
{
public:
    Contact();
    ~Contact();
    void objectOneData(float x, float y, float width, float height);
    void objectTwoData(float x, float y, float width, float height);
    bool testCollision();
private:
    float object_point[2][4] = {0.0f};
};


#endif //ESDLB3_CONTACT_H
