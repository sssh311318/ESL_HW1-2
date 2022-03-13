#include <cmath>

#include "gaussianFilter.h"

gaussianFilter::gaussianFilter(sc_module_name n) : sc_module(n) {
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

// gaussian mask
const float mask[MASK_X][MASK_Y] = {{0.077847, 0.123317, 0.077847}, {0.123317, 0.195346, 0.123317}, {0.077847, 0.123317, 0.077847}};
// row buffer
 double row_buffer[3][258] = {0};


void gaussianFilter::do_filter() {
  int x, y;
  double val_1 = 0;
  double val_2 = 0;
  double val_3 = 0;
  double total_val = 0;
  double total = 0;
  while (true) {
      //val_r= 0; val_g= 0; val_b = 0;
        val_grey = 0;
    /* mask loop
    for (unsigned int v = 0; v < MASK_Y; ++v) {
      for (unsigned int u = 0; u < MASK_X; ++u) {
        */
    for (y = -1; y <= 256; ++y) {
      for (x = -1; x <= 256; ++x) { 
        unsigned char red = i_r.read();
        unsigned char green = i_g.read();
        unsigned char blue = i_b.read();
        wait(1);
        unsigned char grey = ( ( red + green + blue ) / 3 ) ;
        row_buffer[2][x+1]= grey;
        // (calculate conv) val_grey += grey * mask[u][v];
        /*
          val_r +=   (red * mask[u][v]);
          val_g +=   (green * mask[u][v]);
          val_b +=   (blue * mask[u][v]);
          */
        }
        for (unsigned int i = 0 ; i <=255 ; i++){
          for (unsigned int u = 0 ; u <=  2  ; u++){
              val_1 += row_buffer[u][i] * mask[u][0];
              val_2 += row_buffer[u][i+1] * mask[u][1];
              val_3 += row_buffer[u][i+2] * mask[u][2];
          }
            total_val = val_1 + val_2 + val_3;
            total =  std::sqrt( total_val * total_val );
            printf(" total = %f \n" ,total);
            o_result_grey.write(total);
            total_val = 0; val_1 = 0; val_2 = 0; val_3 = 0;
        }
          /*
          if (counter <= 255) counter++;
          else counter = 0;
          for(unsigned int i = 0 ; i <=255 ; i++){
            total =  std::sqrt( val[i] * val[i] );
            printf(" total = %f \n" ,total);
            o_result_grey.write(std::sqrt(total));
            wait(1);
          }
          */
        for (unsigned int i = 0 ; i <=257 ; i++){
          row_buffer[0][i] = row_buffer[1][i];
          row_buffer[1][i] = row_buffer[2][i];
        }
      //for (unsigned int i = 0 ; i <=255 ; i++){

      //}
        

    }    
    /*
    int result_grey = (val_grey * val_grey);
    o_result_grey.write(std::sqrt(result_grey));
    */

 
        
    /*
    int result_r = (val_r * val_r);
    int result_g = (val_g * val_g);
    int result_b = (val_b * val_b);
    o_result_r.write(std::sqrt(result_r) );
    o_result_g.write(std::sqrt(result_g) );
    o_result_b.write(std::sqrt(result_b) );
    */
    //wait(10); //emulate module delay
  }
}
