/* Chapter 14 Challenge 1: compressed source-line information. */
typedef struct {
  int offset;
  int line;
} LineStart;

/* Chunk fields:
   int lineCount;
   int lineCapacity;
   LineStart* lines;
*/

/* In writeChunk(), grow the line array only when the source line changes. */
if (chunk->lineCount == 0 ||
    chunk->lines[chunk->lineCount - 1].line != line) {
  if (chunk->lineCapacity < chunk->lineCount + 1) {
    int oldCapacity = chunk->lineCapacity;
    chunk->lineCapacity = GROW_CAPACITY(oldCapacity);
    chunk->lines = GROW_ARRAY(LineStart, chunk->lines,
        oldCapacity, chunk->lineCapacity);
  }
  chunk->lines[chunk->lineCount++] =
      (LineStart){chunk->count - 1, line};
}

int getLine(Chunk* chunk, int instruction) {
  int start = 0;
  int end = chunk->lineCount - 1;
  while (start <= end) {
    int middle = (start + end) / 2;
    if (instruction < chunk->lines[middle].offset) {
      end = middle - 1;
    } else if (middle == chunk->lineCount - 1 ||
               instruction < chunk->lines[middle + 1].offset) {
      return chunk->lines[middle].line;
    } else {
      start = middle + 1;
    }
  }
  return -1;
}
