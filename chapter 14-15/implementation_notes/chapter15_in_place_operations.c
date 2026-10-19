/* Chapter 15 Challenge 4: avoid unnecessary pop/push operations. */
case OP_NEGATE:
  if (!IS_NUMBER(stackTop[-1])) {
    runtimeError("Operand must be a number.");
    return INTERPRET_RUNTIME_ERROR;
  }
  stackTop[-1] = NUMBER_VAL(-AS_NUMBER(stackTop[-1]));
  break;

/* Binary arithmetic can similarly pop only the right operand and overwrite
   the left operand. OP_NOT can replace stackTop[-1] directly as well. */
