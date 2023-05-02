#pragma once

#include "Arduino.h"
#include "LedController.hpp"

namespace sakurajin{

  template<size_t ROWS>
  class segmentDisplay{
    private:
    LedController<1,ROWS> segmentController;

    public:
      segmentDisplay(const controller_configuration<1,ROWS>& conf):segmentController{conf}{
        segmentController.setIntensity(0);
        for(unsigned int i = 0; i < 8;i++){
          for(size_t j = 0; j < ROWS; j++){
            segmentController.setRow(j,i,0x00);
          }
        }
      }

      LedController<1,ROWS>& controller(){
        return segmentController;
      }

      template <typename T>
      void display(T value, unsigned int row = 0, uint8_t decimalPlaces = 1, uint8_t digitOffset = 0, uint8_t digits = 4){
        uint64_t total_length = digits;
        if(digits<decimalPlaces){return;};

        //display a minus and thus reduce the length
        if(value < 0){
          segmentController.setChar(row,total_length-1+digitOffset,'-',false);
          total_length--;
        };

        //shift the number by the amount of places that will be displayed
        for(unsigned int i = 0; i < decimalPlaces; i ++){
          value *= static_cast<T>(10);
        }

        uint64_t v = static_cast<uint64_t>(value < -value ? -value : value);

        for (unsigned int i = 0; i < total_length;i++){
          segmentController.setDigit(row,i+digitOffset,v%10,i == decimalPlaces);
          v /= 10;
        }
      }
  };
}