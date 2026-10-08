
#ifndef HELPER_HPP
#define HELPER_HPP

#include <fcntl.h>
#include <unistd.h>

class RAII_FD
{
  public:
    explicit RAII_FD(int fd) : fd_{fd}
    {
    }

    ~RAII_FD()
    {
        if (fd_ >= 0)
        {
            close(fd_);
        }
    }

    RAII_FD(const RAII_FD &) = delete;
    RAII_FD &operator=(const RAII_FD &) = delete;

    RAII_FD(RAII_FD &&other) noexcept : fd_{other.fd_}
    {
        other.fd_ = -1;
    }

    int get() const
    {
        return fd_;
    }

  private:
    int fd_{-1};
};

#endif // HELPER_HPP
