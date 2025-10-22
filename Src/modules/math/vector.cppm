module;

//----------------------------------------------------------------------------------------------------------------------------

#include <cmath>

import compare;

//----------------------------------------------------------------------------------------------------------------------------

export module vector;

//----------------------------------------------------------------------------------------------------------------------------

export namespace Math
{

//----------------------------------------------------------------------------------------------------------------------------

namespace LinearAlgebra
{

//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
class vector_2_t
{
    private:
        number_t a1_;
        number_t a2_;

    public:
        vector_2_t();
        vector_2_t(const number_t& a1, const number_t& a2);

        void       set_a1                   (const number_t& a1_new_value);
        void       set_a2                   (const number_t& a2_new_value);

        void       mul_by_number            (const number_t& number);

        number_t   get_a1                   ()                         const;
        number_t   get_a2                   ()                         const;

        vector_2_t get_mul_by_number        (const number_t& number)   const;
        number_t   get_mul_by_another_vector(const vector_2_t& vector) const;
        number_t   get_square_module        ()                         const;
        number_t   get_module               ()                         const;

        bool       compare                  (const vector_2_t& vector) const;

        bool       is_valid                 ()                          const;
};

//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
vector_2_t<number_t>::vector_2_t() :
a1_(),
a2_()
{}

//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
vector_2_t<number_t>::vector_2_t(const number_t& a1, const number_t& a2) :
a1_(a1),
a2_(a2)
{}

//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
number_t
vector_2_t<number_t>::get_a1() const
{
    return a1_;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
number_t
vector_2_t<number_t>::get_a2() const
{
    return a2_;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
void
vector_2_t<number_t>::set_a1(const number_t& a1_new_value)
{
    a1_ = a1_new_value;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
void
vector_2_t<number_t>::set_a2(const number_t& a2_new_value)
{
    a2_ = a2_new_value;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
void
vector_2_t<number_t>::mul_by_number(const number_t& number)
{
    a1_ *= number;
    a2_ *= number;
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
vector_2_t<number_t>
vector_2_t<number_t>::get_mul_by_number(const number_t& number) const
{
    return vector_2_t(a1_ * number, a2_ * number);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
number_t
vector_2_t<number_t>::get_mul_by_another_vector(const vector_2_t& vector) const
{
    return
        (a1_ * vector.a1_) +
        (a2_ * vector.a2_);
}


//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
number_t
vector_2_t<number_t>::get_square_module() const
{
    return
        (a1_ * a1_) +
        (a2_ * a2_);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
number_t
vector_2_t<number_t>::get_module() const
{
    return std::sqrt(get_square_module());
}


//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
bool
vector_2_t<number_t>::compare(const vector_2_t& vector) const
{
    return
        compare<number_t>(a1_, vector.a1_) &&
        compare<number_t>(a2_, vector.a2_);
}

//----------------------------------------------------------------------------------------------------------------------------

template <typename number_t>
bool
vector_2_t<number_t>::is_valid() const
{
    return 
        !std::isnan(a1_) &&
        !std::isnan(a2_);
}

//----------------------------------------------------------------------------------------------------------------------------

} /* namespace LinearAlgebra */
} /* namespace Math */

//----------------------------------------------------------------------------------------------------------------------------
