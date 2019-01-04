/**
 * Created by Teddy Juntunen
 * 12 November 2018
 * 
 * Grep implementation. This file handles matching the pattern to the
 * input its given recursively. 
 */

#include "matcher.h"
#include <stdio.h>

// Returns the next char from given char pointer.
char next_char(char* str) {
	return *(str + 1);
}

// Returns the previous char from the given char pointer.
char previous_char(char* str) {
	return *(str - 1);
}

// Returns true if the given char is a plus sign char.
int is_plus_operator(char c) {
	return c == '+';
}

// Returns true if the given char is a period.
int is_dot_operator(char c) {
	return c == '.';
}

// Returns true if the given char is a question mark.
int is_question_operator(char c) {
	return c == '?';
}


// Returns true if the given char is a backslash.
int is_backslash_operator(char c) {
	return c == '\\';
}


/**
 * Returns true if partial_line matches pattern, starting from
 * the first char of partial_line.
 */
int matches_leading(char *partial_line, char *pattern) {
	char c = *partial_line;
	char pat = *pattern;

	// If c is null, return 1 if the pattern is also null - and 0 if it's not.
	if(!c) { return pat ? 0 : 1; }

	// If you reach the end of the pattern while c is not null, return 1.
	if(!pat) { return 1; }

	if(is_backslash_operator(pat)) {
		pattern++; // Increment past the backslash char.

		// Check equality of line and pattern chars without operator effects.
		if(c != *pattern) { return 0; }
	}

	if(is_question_operator(next_char(pattern))) {
		if(c != pat) { // If char shows up 0 times.
			pat = *(pattern + 2); // Increment pattern to sync up with line.
			if(!pat) { return 1; } // Return 1 because there's no char after the question mark.
			if(c != pat) { return 0; }
			pattern += 2; // Skip to the char after the question mark.
		} else {
			if(c != pat) { return 0; }
			pattern++; // Skip to the char after the question mark.
		}
	}

	if(is_plus_operator(next_char(pattern))) {
		if(c != pat) { return 0; }
		if(c == next_char(partial_line)) { // The sequence of the same char continues.
			pattern--; // Keep pattern where it is for the next iteration since the sequence is not over.
		} else {
			pattern++; // Continue on with the rest of the pattern.
		}
	}

	/*
	 * Check for equality between the current pattern char and the current line char,
	 * if current pattern char is a dot '.', or current char is backslash, then
	 * equalness is ignored. 
	 */
	if(c != pat && !is_dot_operator(pat) && !is_backslash_operator(pat)) {
		return 0;
	}
	return matches_leading(partial_line + 1, pattern + 1);
}

/**
 * Implementation of your matcher function, which
 * will be called by the main program.
 *
 * You may assume that both line and pattern point
 * to reasonably short, null-terminated strings.
 */
int rgrep_matches(char *line, char *pattern) {
	while(*line != '\0') {
		if(matches_leading(line++, pattern)) {
			return 1;
		}
	}
	return 0;
}
