#include <vector>

class Permutation {
public:
    Permutation(unsigned int size);

    char Sign() const;
    const std::vector<unsigned int>& GenerateNext();
private:
    std::vector<unsigned int> data_;
    char sign_;
};