#if !defined(Singleton_define)
#define Singleton_define

template <typename T>   
  class   Singleton   
  {   
  public:   
      static T * getInst(){   
		  //if(!ptr_){
		  if(0 == cnt_){
			  ptr_ = new T();   
		  }
		  ++cnt_;   
		  return ptr_;   
	  }   
      static   void   releaseInst(T * _p){
		  if(0 == --cnt_){
			  delete ptr_;  
			  ptr_ = 0;
		  }
          if (cnt_ < 0)
          {
              cnt_ = 0;
          }
	  }
	  static T * getInst(HWND hwnd){
		  //if (!ptr_)
		  if (0 == cnt_)
		  {
			  ptr_ = new T(hwnd);
		  }
		  ++cnt_;
		  return ptr_;
	  }
  private:
      static T * ptr_;
      static size_t cnt_;
  };   

#endif
