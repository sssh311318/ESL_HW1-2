#include <cmath>

#include "gaussianFilter.h"
gaussianFilter::gaussianFilter(sc_module_name n) : sc_module(n) {

  SC_THREAD(buffer);
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

// gaussian mask
const float mask[MASK_X][MASK_Y] = {{0.077847, 0.123317, 0.077847}, {0.123317, 0.195346, 0.123317}, {0.077847, 0.123317, 0.077847}};

unsigned char row_buffer[256][256] = {};
void gaussianFilter::buffer(){
  for( unsigned char pixel_x = 0; pixel_x <= 256; pixel_x++){
    for ( unsigned char pixel_y = 0; pixel_y <= 256; pixel_y++){
      row_buffer[pixel_x][pixel_y] = i_grey.read();
      if(pixel_y <= 254)printf(" %d ",row_buffer[pixel_x][pixel_y]);
      else {printf("\n pixel_y is done\n");
            printf("--------------------------------------------------------------\n");
      }
    }  
  }
}
void gaussianFilter::do_filter(){
    unsigned int val = 0;
    for(unsigned int x = 0; x < 256; x++){
      for(unsigned int y = 0; y < 256; y++){
        for(unsigned int u = 0; u < 3; u++){
          for(unsigned int v = 0; v < 3; v++){
            val += row_buffer[x][y] * mask[u][v];
          }
        }
      double total = 0;
      total = val * val ;
      int result = (int)(std::sqrt(total));
      o_result.write(result);
     
      }
    }
  }
/*void gaussianFilter::do_filter() {
  while (true) {
    unsigned int val = 0;
    for (unsigned int v = 0; v < MASK_Y; ++v) {
      for (unsigned int u = 0; u < MASK_X; ++u) {
        //unsigned char grey = (i_r.read() + i_g.read() + i_b.read()) / 3;
          unsigned char grey = i_grey.read();
          val += grey * mask[u][v];
      }
    }
    double total = 0;
    total = val * val ;
    int result = (int)(std::sqrt(total));
    o_result.write(result);
    
    wait(10); //emulate module delay
  }
}*/
