trait AnyField {
}

struct Field<T, const N: usize> {
    data: [T; N],
}

impl<T, const N: usize> AnyField for Field<T, N> {
}

// type Name = Field<str, 1>;
type Data = Field<u8, 4>;
type Header = Field<u8, 2>;

struct Packet {
    fields: Vec<Box<dyn AnyField>>,
}

fn main() {
    let packet = Packet {
        fields: vec![
            Box::new(Data { data: [0, 1, 2, 3] }),
            Box::new(Header { data: [0, 1] }),
        ],
    };
}
