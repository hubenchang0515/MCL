#ifndef MCL_TENSOR_H
#define MCL_TENSOR_H

#include <MCL/MCL.h>
#include "initializer_list"

class Tensor
{
public:
    ~Tensor() noexcept;
    Tensor(size_t shape[], size_t dim) noexcept;
    Tensor(std::initializer_list<size_t> shape);
    Tensor(const Tensor& src) noexcept;
    Tensor(Tensor&& src) noexcept;

    static bool init(MCL::Device& dev) noexcept;

    size_t* shape(size_t* dim=nullptr) const noexcept;
    size_t dim() const noexcept;
    cl_float* data() const noexcept;
    size_t size() const noexcept;

    void printShape() const noexcept;
    void printData() const noexcept;

    Tensor add(const Tensor& n) const noexcept;

private:
    size_t*    m_shape;
    size_t      m_dim;
    cl_float*   m_data;
    size_t      m_size;


    static MCL::Device clDev;
    static MCL::Context clCtx;
    static MCL::CommandQueue clCmd;
    static MCL::Program clProgram;
    static MCL::Kernel clAdd;
    static void print(cl_float* data, size_t* shape, size_t dim, size_t tab=0) noexcept;
};

#endif
