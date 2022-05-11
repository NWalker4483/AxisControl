
#ifndef Helpers_h
#define Helpers_h
template <typename num>
inline num constrain(num val, num min, num max)
{
  if (val < min)
    return min;
  if (val > max)
    return max;
  return val;
}

template <typename num>
num approach(num curr, num min, num max, num step)
{
  /*
  curr should be added to step but with the caveat that it
  cannot decrease below min and cannot increase above max
  Decreasing to a value above max will not cause any constraints and vice versa for increasing to a value below minimum
  */
  if (step > 0)
  {
    return constrain(curr + step, curr, max);
  }
  else
  {
    return constrain(curr + step, min, curr);
  }
}

template <typename num>
inline num sign(num curr)
{
  return curr >= 0 ? 1L : -1L;
}
#endif