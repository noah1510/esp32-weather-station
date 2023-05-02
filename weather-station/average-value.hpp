

namespace sakurajin{
  template<typename T, size_t SIZE>
  class averageValue{
    private:
      T samples[SIZE];

    public:
      averageValue() = default;
      averageValue(const T& initValue){
        for(size_t i = 0; i < SIZE;i++){
          samples[i] = initValue;
        }
      }

      T& operator[](size_t index){
        static_assert(index < SIZE);
        return samples[index];
      }

      const T& operator[](size_t index) const{
        static_assert(index < SIZE);
        return samples[index];
      }

      T getAverage() const{
        T val = samples[0];
        for(size_t i = 1; i < SIZE; i++){
          val += samples[i];
        }
        return val/SIZE;
      }
  }
}
