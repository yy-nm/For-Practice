package bmstringsearch

import "testing"

func TestSearch(t *testing.T) {
	testcase := []struct {
		content, pat string
		match        bool
	}{
		{
			"ABCXXABCXXXABC",
			"ABCXXXABC",
			true,
		},
		{
			"ABCXXABCXXXABC",
			"ABYXCDEYX",
			false,
		},
		{
			"",
			"",
			false,
		},
		{
			content: "hello, world",
			pat:     "dlrow ,olleh",
			match:   false,
		},
		{
			"Hello world",
			"Hello",
			true,
		},
		{
			"Hello world",
			"world",
			true,
		},
		{
			"Hello world",
			"o wo",
			true,
		},
		{
			"Hello world",
			"not",
			false,
		},
		{
			"svertices vertices",
			"vertices",
			true,
		},
		{
			"FFS :svertices :vertices",
			":vertices",
			true,
		},
		{
			"cabadccabcdab",
			"ccabcdab",
			true,
		},
	}

	for _, c := range testcase {
		match := Search(c.content, c.pat)
		if match != c.match {
			t.Errorf("search(%q) in %q, result: %q, true is :%q", c.pat, c.content, match, c.match)
		}
	}
}
