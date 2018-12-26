package stringhelper

import "testing"

func TestReverseString(t *testing.T) {
	testcase := []struct {
		in, out string
	}{
		{"hello, world", "dlrow ,olleh"},
		{"你好", "好你"},
		{"", ""},
	}

	for _, c := range testcase {
		out := ReverseString(c.in)
		if out != c.out {
			t.Errorf("Reverse(%q) == %q, true is :%q", c.in, out, c.out)
		}
	}
}
