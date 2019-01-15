package bmstringsearch

import "testing"

func TestSearch(t *testing.T) {
	testcase := []struct {
		content, pat string
		match        int
	}{
		{
			"ABCXXABCXXXABC",
			"ABCXXXABC",
			5,
		},
		{
			"ABCXXABCXXXABC",
			"ABYXCDEYX",
			-1,
		},
		{
			"",
			"",
			-1,
		},
		{
			content: "hello, world",
			pat:     "dlrow ,olleh",
			match:   -1,
		},
		{
			"Hello world",
			"Hello",
			0,
		},
		{
			"Hello world",
			"world",
			6,
		},
		{
			"Hello world",
			"o wo",
			4,
		},
		{
			"Hello world",
			"not",
			-1,
		},
		{
			"svertices vertices",
			"vertices",
			1,
		},
		{
			"FFS :svertices :vertices",
			":vertices",
			15,
		},
		{
			"cabadccabcdab",
			"ccabcdab",
			5,
		},
	}

	for _, c := range testcase {
		match := Search(c.content, c.pat)
		if match != c.match {
			t.Errorf("search(%q) in %q, result: %v, true is :%v", c.pat, c.content, match, c.match)
		}
	}
}
