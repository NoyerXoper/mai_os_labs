#include <vector>

class Permutation {
public:
    Permutation(unsigned int size);
    Permutation(unsigned int size, std::size_t i);

    char Sign() const noexcept;
    const std::vector<unsigned int>& GetCurrent() const noexcept;
    const std::vector<unsigned int>& GenerateNext();
    void ResetFirst() noexcept;
    bool IsEnd() const noexcept;
    void Swap(Permutation& other);
    friend bool operator==(const Permutation& first, const Permutation& second) noexcept;
    friend bool operator!=(const Permutation& first, const Permutation& second) noexcept;
private:
    std::vector<unsigned int> data_;
    char sign_;
    bool isFirst_;
    bool isEnd_;
};

bool operator==(const Permutation& first, const Permutation& second) noexcept;
bool operator!=(const Permutation& first, const Permutation& second) noexcept;
