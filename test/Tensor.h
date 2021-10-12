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

    Tensor& operator = (const Tensor& src) noexcept;
    Tensor& operator = (Tensor&& src) noexcept;

    static bool init(MCL::Device& dev) noexcept;

    size_t* shape(size_t* dim=nullptr) const noexcept;
    size_t dim() const noexcept;
    cl_float* data() const noexcept;
    size_t size() const noexcept;

    void printShape() const noexcept;
    void printData() const noexcept;

    Tensor scalar(const Tensor& n, const MCL::Kernel& kernel) const noexcept;
    Tensor add(const Tensor& n) const noexcept;
    Tensor sub(const Tensor& n) const noexcept;
    Tensor mul(const Tensor& n) const noexcept;
    Tensor div(const Tensor& n) const noexcept;

private:
    size_t*     m_shape;
    size_t      m_dim;
    cl_float*   m_data;
    size_t      m_size;

    static void print(cl_float* data, size_t* shape, size_t dim, size_t tab=0) noexcept;

    static MCL::Device clDev;
    static MCL::Context clCtx;
    static MCL::CommandQueue clCmd;
    static MCL::Program clProgram;

    static MCL::Kernel clAdd;
    static MCL::Kernel clSub;
    static MCL::Kernel clMul;
    static MCL::Kernel clDiv;
};

#endif
