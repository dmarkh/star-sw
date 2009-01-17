//
// Pibero Djawotho <pibero@comp.tamu.edu>
// Texas A&M University Cyclotron Institute
// 1 Jan 2009
//

#ifndef DSM_LAYER_HH
#define DSM_LAYER_HH

#include <byteswap.h>
#include <vector>

using namespace std;

#include "DSM.hh"

inline long long swapLL(long long x)
{
  return ((x & 0xffff000000000000ll) >> 48 |
	  (x & 0x0000ffff00000000ll) >> 16 |
	  (x & 0x00000000ffff0000ll) << 16 |
	  (x & 0x000000000000ffffll) << 48);
}

inline void copy_and_swap16(void* dest, const void* src)
{
  long long* x = (long long*)src;
  long long* y = (long long*)dest;

  *y++ = bswap_64(*x++);
  *y++ = bswap_64(*x++);
}

inline void copy_and_swap8(void* dest, const void* src)
{
  long long* x = (long long*)src;
  long long* y = (long long*)dest;

  *y++ = swapLL(*x++);
  *y++ = swapLL(*x++);
}

template<class T> struct DSMLayer : public vector<DSM> {
  DSMLayer(int n) : vector<DSM>(n) {}
  virtual ~DSMLayer() {}
  virtual void setRegister(int i, int value);
  virtual void read(const T& event) = 0;
  virtual void write(DSMLayer& layer) = 0;
  virtual void run() = 0;
  virtual void save(int nchannels, short* buffer);
};

template<class T> inline void DSMLayer<T>::setRegister(int i, int value)
{
  for (vector<DSM>::iterator dsm = begin(); dsm != end(); ++dsm)
    dsm->registers[i] = value;
}

template<class T> inline void DSMLayer<T>::save(int nchannels, short* buffer)
{
  for (size_t dsm = 0; dsm < size(); ++dsm) {
    short* channels = (*this)[dsm].channels;
    copy(&channels[0], &channels[nchannels], &buffer[dsm*nchannels]);
  }
}

#endif	// DSM_LAYER_HH
