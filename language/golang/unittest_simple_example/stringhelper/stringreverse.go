package stringhelper

import "bytes"

func ReverseString(s string) string {

	runearray := make([]rune, 0)
	var result bytes.Buffer
	for _, runevalue := range s {
		runearray = append(runearray, runevalue)
	}

	if len(runearray) == 0 {
		return ""
	}

	for i := len(runearray) - 1; i >= 0; i-- {
		result.WriteRune(runearray[i])
	}
	return result.String()
}
