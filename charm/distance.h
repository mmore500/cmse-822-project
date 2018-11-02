#include <algorithm>
#include <cmath>

#ifndef __DISTANCE_H__
#define __DISTANCE_H__

/*
 * Calculate toroidal distance along one dimension.
 */
size_t inline distToroidal(size_t c_1, size_t c_2, size_t toroidRadius) {
  size_t dc = c_1 > c_2 ? c_1 - c_2 : c_2 - c_1;
  return std::min(dc, toroidRadius - dc);
};

/*
 * Calculate taxi cab distance in two dimensions.
 */
size_t inline taxiCab(
    size_t x_1, size_t y_1,
    size_t x_2, size_t y_2,
    size_t toroidRadiusX, size_t toroidRadiusY
  ) {

  return distToroidal(x_1, x_2, toroidRadiusX)
          + distToroidal(y_1, y_2, toroidRadiusY);

};

/*
 * Calculate euclidean distance in two dimensions
 */
double inline euclidean(
    size_t x_1, size_t y_1,
    size_t x_2, size_t y_2,
    size_t toroidRadiusX, size_t toroidRadiusY
  ) {

  size_t dx = distToroidal(x_1, x_2, toroidRadiusX);
  size_t dy = distToroidal(y_1, y_2, toroidRadiusY);

  return sqrt((dx * dx) + (dy * dy));

};

/*
 * Calculate distance in two dimensions.
 * Could be implemented several ways.
 */
double inline distance(
    size_t x_1, size_t y_1,
    size_t x_2, size_t y_2,
    size_t toroidRadiusX, size_t toroidRadiusY
  ) {
  return taxiCab(x_1, y_1, x_2, y_2, toroidRadiusX, toroidRadiusY);
};

#endif //__DISTANCE_H__
