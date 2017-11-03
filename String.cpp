#include <iostream>

#include "String.h"


const int BUFFSIZE = 8;

// -------------- REAL IMPLEMENTATION --------------
class String::Impl {
public:
    Impl() : sso_(true) { }

    Impl(const char* arg) {
        size_t arg_len = strlen(arg);

        if(arg_len < BUFFSIZE){
            strcpy(changeable_buffer_.short_, arg);
            sso_ = true;
        }
        else{
            changeable_buffer_.long_ = new char[arg_len + 1];
            strcpy(changeable_buffer_.long_, arg);
            sso_ = false;
        }
    }

    Impl(const Impl& rhs){
        if(rhs.sso_){
            strcpy(changeable_buffer_.short_, rhs.changeable_buffer_.short_);
            sso_ = true;
        }
        else{
            changeable_buffer_.long_ = new char[strlen(rhs.changeable_buffer_.long_) + 1];
            strcpy(changeable_buffer_.long_, rhs.changeable_buffer_.long_);
            sso_ = false;
        }
    }

    Impl& operator=(const Impl& rhs){
        if(!sso_){
            delete [] changeable_buffer_.long_;
        }
        if(rhs.sso_){
            strcpy(changeable_buffer_.short_, rhs.changeable_buffer_.short_);
        }
        else{
            changeable_buffer_.long_ = new char[strlen(rhs.changeable_buffer_.long_) + 1];
            strcpy(changeable_buffer_.long_, rhs.changeable_buffer_.long_);
        }
        sso_ = rhs.sso_;
        return *this;
    }

    ~Impl() {
        if(!sso_){
            delete [] changeable_buffer_.long_;
        }
    }

    friend std::ostream& operator<<(std::ostream& str, const Impl& arg) {
        if (arg.sso_) {
            str << arg.changeable_buffer_.short_;
        }
        else{
            str << arg.changeable_buffer_.long_;
        }
        return str;
    }

private:
    union Buffer{
        Buffer() : long_(nullptr) { }

        char* long_;
        char short_[BUFFSIZE];
    } changeable_buffer_;

    bool sso_;
};

// -------------- WRAPPER --------------
String::String() : pimpl_( new Impl() ) { }

String::String(const char* arg) : pimpl_( new Impl(arg) ) { }

String::String(const String& rhs) : pimpl_( new Impl(*(rhs.pimpl_)) ) { }

String& String::operator=(const String& rhs) {
    *pimpl_ = *(rhs.pimpl_);
    return *this;
}

String::~String() {
    delete pimpl_;
}

void String::swap(String& rhs) {
    Impl* tmp = pimpl_;
    pimpl_ = rhs.pimpl_;
    rhs.pimpl_ = tmp;
}

std::ostream& operator<<(std::ostream& str, const String& arg) {
    str << *(arg.pimpl_);
    return str;
}


int main(){
    //--------REFERENCE TESTS-----------
    String s1;
    String s2("Hello world");

    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;

    s1 = s2;
    std::cout << s1 << std::endl;

    String s3(s1);
    std::cout << s3 << std::endl;

    String s4("Short");
    std::cout << s4 << std::endl;
    s4.swap(s3);
    std::cout << s3 << std::endl;
    std::cout << s4 << std::endl << "------------------------" << std::endl;
    //---------------------------------------

    //------------- MY TESTS ----------------
    // Inicializace ruznyh delek (init of various sizes)
    String a("short1");
    std::cout << "a: " << a << std::endl;

    String b("short2");
    std::cout << "b: " << b << std::endl;

    String c("megareallylong1");
    std::cout << "c: " << c << std::endl;

    String d("megareallylong2");
    std::cout << "d: " << d << std::endl;

    // Kopirovaci konstruktor (copy constructor)
    String x(a);
    std::cout << "x: " << x << std::endl;

    String y(c);
    std::cout << "y: " << y << std::endl;

    // Kopirovaci prirazovaci operator (copy assignment operator)
    String r;
    std::cout << "r: " << r << std::endl;
    r = b;
    std::cout << "r: " << r << std::endl;
    r = d;
    std::cout << "r: " << r << std::endl;
    r = a;
    std::cout << "r: " << r << std::endl;
    r = b;
    std::cout << "r: " << r << std::endl;
    r = c;
    std::cout << "r: " << r << std::endl;
    r = d;
    std::cout << "r: " << r << std::endl;

    // Konverzni konstruktor (conversion constructor)
    String u = "konver";
    std::cout << "u: " << u << std::endl;
    String k = "testujudlouhykonverzni";
    std::cout << "k: " << k << std::endl;

    return 0;
}
