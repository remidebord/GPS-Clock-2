// (most significant bit) dot g f e d c b a (Least significant bit))
//                    A -> 0  1 1 1 0 1 1 1 = 0x77 

const char digitAsciiMap[128] = {0x00, // Null
																 0x00, // Start of heading
																 0x00, // Start of text
																 0x00, // End of text
																 0x00, // End of transmission
																 0x00, // Enquiry
																 0x00, // Acknowledge
																 0x00, // Bell
																 0x00, // Backspace
																 0x00, // Horizontal tab
																 0x00, // Line feed
																 0x00, // Vertical tab
																 0x00, // Form feed
																 0x00, // Carriage return
																 0x00, // Shift out
																 0x00, // Shift in
																 0x00, // Data link escape
																 0x00, // Device control 1
																 0x00, // Device control 2
																 0x00, // Device control 3
																 0x00, // Device control 4
																 0x00, // Negative acknowledge
																 0x00, // Synchronous idle
																 0x00, // Eng of trans. block
																 0x00, // Cancel
																 0x00, // End of medium
																 0x00, // Substitute
																 0x00, // Escape
																 0x00, // File separator
																 0x00, // Group separator
																 0x00, // Record separator
																 0x00, // Unit separator
																 0x00, // Space
																 0x00, // !
																 0x22, // "
																 0x00, // #
																 0x00, // $
																 0x00, // %
																 0x00, // &
																 0x20, // '
																 0x39, // (
																 0x0F, // )
																 0x00, // *
																 0x00, // +
																 0x80, // ,
																 0x40, // -
																 0x80, // .
																 0x00, // antislash
																 0x3F, // 0
																 0x06, // 1
																 0x5B, // 2
																 0x4F, // 3
																 0x66, // 4
																 0x6D, // 5
																 0x7D, // 6
																 0x07, // 7
																 0x7F, // 8
																 0x6F, // 9
																 0x00, // :
																 0x00, // ;
																 0x00, // <
																 0x48, // =
																 0x00, // >
																 0x00, // ?
																 0x00, // @
																 0x77, // A
																 0x7C, // B
																 0x39, // C
																 0x5E, // D
																 0x79, // E
																 0x71, // F
																 0x6F, // G
																 0x76, // H
																 0x30, // I
																 0x1E, // J
																 0x76, // K
																 0x38, // L
																 0x15, // M
																 0x54, // N
																 0x3F, // O
																 0x73, // P
																 0x67, // Q
																 0x50, // R
																 0x6D, // S
																 0x78, // T
																 0x3E, // U
																 0x00, // V
																 0x00, // W
																 0x76, // X
																 0x6E, // Y
																 0x5B, // Z
																 0x39, // [
																 0x00, // slash
																 0x0F, // ]
																 0x00, // ^
																 0x08, // _
																 0x20, // `
																 0x77, // a
																 0x7C, // b
																 0x58, // c
																 0x5E, // d
																 0x79, // e
																 0x71, // f
																 0x6F, // g
																 0x74, // h
																 0x10, // i
																 0x1E, // j
																 0x76, // k
																 0x38, // l
																 0x00, // m
																 0x54, // n
																 0x5C, // o
																 0x73, // p
																 0x67, // q
																 0x50, // r
																 0x6D, // s
																 0x78, // t
																 0x1C, // u
																 0x00, // v
																 0x00, // w
																 0x76, // x
																 0x6E, // y
																 0x5B, // z
																 0x00, // {
																 0x30, // |
																 0x00, // }
																 0x00, // ~
																 0x00, // Del
																 };
