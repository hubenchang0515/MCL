#include "Tensor.h"

MCL::Device Tensor::clDev = MCL::Device::invalid;
MCL::Context Tensor::clCtx = MCL::Context::invalid;
MCL::CommandQueue Tensor::clCmd = MCL::CommandQueue::invalid;
MCL::Program Tensor::clProgram = MCL::Program::invalid;
MCL::Kernel Tensor::clAdd = MCL::Kernel::invalid;
MCL::Kernel Tensor::clSub = MCL::Kernel::invalid;
MCL::Kernel Tensor::clMul = MCL::Kernel::invalid;
MCL::Kernel Tensor::clDiv = MCL::Kernel::invalid;

static void printTab(size_t tab) noexcept
{
    for(size_t i = 0; i < tab; i++)
        printf(" ");
}

bool Tensor::init(MCL::Device& dev) noexcept
{
    Tensor::clDev = dev;
    if (clDev.id() == nullptr)
    {
        fprintf(stderr, "failed to find dev\n");
        return false;
    }
    
    Tensor::clCtx = MCL::Context::create(dev);
    if (clCtx.id() == nullptr)
    {
        fprintf(stderr, "failed to create context\n");
        return false;
    }
    
    Tensor::clCmd = MCL::CommandQueue::create(clDev, clCtx);
    if (clCmd.id() == nullptr)
    {
        fprintf(stderr, "failed to create command queue\n");
        return false;
    }

    Tensor::clProgram = MCL::Program::load(clCtx, "kernel.cl");
    if (!clProgram.build(clDev))
    {
        fprintf(stderr, "failed to build program:\n%s\n", clProgram.info(clDev).c_str());
        return false;
    }

    Tensor::clAdd = MCL::Kernel::create(clProgram, "add");
    if (clAdd.id() == nullptr)
    {
        fprintf(stderr, "failed to create kernel add\n");
        return false;
    }

    Tensor::clSub = MCL::Kernel::create(clProgram, "sub");
    if (clSub.id() == nullptr)
    {
        fprintf(stderr, "failed to create kernel sub\n");
        return false;
    }

    Tensor::clMul = MCL::Kernel::create(clProgram, "mul");
    if (clMul.id() == nullptr)
    {
        fprintf(stderr, "failed to create kernel mul\n");
        return false;
    }

    Tensor::clDiv = MCL::Kernel::create(clProgram, "div");
    if (clDiv.id() == nullptr)
    {
        fprintf(stderr, "failed to create kernel div\n");
        return false;
    }

    return true;
}

void Tensor::print(cl_float* data, size_t* shape, size_t dim, size_t tab) noexcept
{
    if (dim == 1)
    {
        printTab(tab);
        printf("[");
        for (size_t i = 0; i < shape[0]; i++)
        {
            printf("%f, ", data[i]);
        }
        printf("]\n");
    }
    else
    {
        printTab(tab);
        printf("[\n");
        size_t subsize = 1;
        for (size_t i = 1; i < dim; i++)
        {
            subsize *= shape[i];
        }
        for (size_t i = 0; i < shape[0]; i++)
        {   
            Tensor::print(data + i * subsize, shape+1, dim-1, tab+1);
        }
        printTab(tab);
        printf("]\n");
    }
}

Tensor::~Tensor() noexcept
{
    if (m_shape != nullptr)
    {
        delete[] m_shape;
        m_shape = nullptr;
    }

    if (m_data != nullptr)
    {
        delete[] m_data;
        m_data = nullptr;
    }

    m_dim = 0;
    m_size = 0;
}

Tensor::Tensor(size_t shape[], size_t dim) noexcept :
    m_dim(dim)
{
    if (m_dim == 0)
        return;

    m_shape = new size_t[dim];
    m_size = 1;
    for (size_t i = 0; i < dim; i++)
    {
        m_shape[i] = shape[i];
        m_size *= shape[i];
    }

    m_data = new cl_float[m_size];
}

Tensor::Tensor(std::initializer_list<size_t> shape):
    m_dim(shape.size())
{
    if (m_dim == 0)
        return;

    m_shape = new size_t[m_dim];
    m_size = 1;
    size_t index = 0;
    for (size_t i : shape)
    {
        m_shape[index] = i;
        m_size *= i;
        index++;
    }

    m_data = new cl_float[m_size];
}

Tensor::Tensor(const Tensor& src) noexcept :
    Tensor(src.m_shape, src.m_dim)
{
    memcpy(m_data, src.m_data, m_size);
}

Tensor::Tensor(Tensor&& src) noexcept : 
    m_shape(src.m_shape),
    m_dim(src.m_dim),
    m_data(src.m_data),
    m_size(src.m_size)
{
    src.m_shape = nullptr;
    src.m_dim = 0;
    src.m_data = nullptr;
    src.m_size = 0;
}

Tensor& Tensor::operator = (const Tensor& src) noexcept
{
    Tensor(src.m_shape, src.m_dim);
    memcpy(m_data, src.m_data, m_size);

    return *this;
}

Tensor& Tensor::operator = (Tensor&& src) noexcept
{
    m_shape = src.m_shape;
    m_dim = src.m_dim;
    m_data = src.m_data;
    m_size = src.m_size;

    src.m_shape = nullptr;
    src.m_dim = 0;
    src.m_data = nullptr;
    src.m_size = 0;

    return *this;
}

size_t* Tensor::shape(size_t* dim) const noexcept
{
    if (dim != nullptr)
        *dim = m_dim;
    return m_shape;
}

size_t Tensor::dim() const noexcept
{
    return m_dim;
}

cl_float* Tensor::data() const noexcept
{
    return m_data;
}

size_t Tensor::size() const noexcept
{
    return m_size;
}

void Tensor::printShape() const noexcept
{
    printf("[");
    for (size_t i = 0; i < m_dim; i++)
    {
        printf("%zu, ", m_shape[i]);
    }
    printf("]\n");
}

void Tensor::printData() const noexcept
{
    Tensor::print(m_data, m_shape, m_dim);
}

Tensor Tensor::scalar(const Tensor& n, const MCL::Kernel& kernel) const noexcept
{
    Tensor result{m_shape, m_dim};

    auto in1 = MCL::Buffer::create(clCtx, m_size * sizeof(cl_float));
    auto in2 = MCL::Buffer::create(clCtx, m_size * sizeof(cl_float));
    auto out = MCL::Buffer::create(clCtx, m_size * sizeof(cl_float));

    in1.write(clCmd, m_data, m_size * sizeof(cl_float));
    in2.write(clCmd, n.m_data, m_size * sizeof(cl_float));

    kernel.setArg(0, in1);
    kernel.setArg(1, in2);
    kernel.setArg(2, out);
    kernel.invoke(clCmd, m_size, 64);

    out.read(clCmd, result.m_data, m_size * sizeof(cl_float));

    return result;
}

Tensor Tensor::add(const Tensor& n) const noexcept
{
    return this->scalar(n, clAdd);
}

Tensor Tensor::sub(const Tensor& n) const noexcept
{
    return this->scalar(n, clSub);
}

Tensor Tensor::mul(const Tensor& n) const noexcept
{
    return this->scalar(n, clMul);
}

Tensor Tensor::div(const Tensor& n) const noexcept
{
    return this->scalar(n, clDiv);
}