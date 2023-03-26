/**
 * The idea here is that the underlying data (residing in Field<N>) is accessed
 * via iterators.
 *
 * See https://stackoverflow.com/q/69634598
 */
#include <array>
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

using value_t = uint8_t;

template<size_t N>
using field_t = array<value_t, N>;

class IField
{
  public:
    virtual ~IField() = default;
    virtual value_t *begin() = 0;
    virtual value_t *end() = 0;
    virtual value_t operator[](size_t index) = 0;
};

template<size_t N>
class Field : public IField
{
  public:
    field_t<N> data;

    template<class... Args>
    Field(Args &&...values)
    {
        static_assert(sizeof...(values) == N);
        this->data = field_t<N>{ values... };
    }

    value_t *begin() override
    {
        return data.data();
    }

    value_t *end() override
    {
        return data.data() + data.size();
    }

    value_t operator[](size_t index) override
    {
        return data[index];
    }
};

class Packet
{
  private:
    vector<unique_ptr<IField>> fields;

  public:
    Packet()
    {
    }

    void add_field(unique_ptr<IField> &field)
    {
        fields.push_back(move(field));
    }

    void add_field(unique_ptr<IField> &&field)
    {
        fields.push_back(move(field));
    }

    IField &operator[](size_t index)
    {
        if (index >= fields.size()) {
            throw std::out_of_range("Index out of range");
        }
        return *fields[index];
    }
};

using Foo = Field<2>;
using Bar = Field<4>;

int main()
{
    Packet p;

    // this will not compile
    // unique_ptr<IField> h(new Field<4>({'a'}));

    p.add_field(make_unique<Foo>(1, 2));

    unique_ptr<IField> f(new Foo({ 3, 4 }));
    p.add_field(f);

    p.add_field(make_unique<Bar>('a', 'b', 'c', 'd'));

    // read first element from the first field of p
    value_t y = p[2][0];
    cout << y << endl;

    return 0;
}
