package ssl.generator;

//@formatter:off
public class OP {

    public final static short

                              NOOP = ((short) 0x8000),
            CONST = ((short) 0x8001),
            CRITICAL_START = ((short) 0x8002),
            CRITICAL_DONE = ((short) 0x8003),
            JMP = ((short) 0x8004),
            CALL = ((short) 0x8005),
            CALL_AT = ((short) 0x8006), // timed function calls
            CALL_CONDITION = ((short) 0x8007),
            CALLSTART = ((short) 0x8008),
            EXEC = ((short) 0x8009),
            SPAWN = ((short) 0x800A),
            FORK = ((short) 0x800B),
            A_TO_D = ((short) 0x800C),
            D_TO_A = ((short) 0x800D),
            EXIT = ((short) 0x800E), // exit prog and purge it from process
                                     // queue
            DETACH = ((short) 0x800F), // detach spawned process from its
                                       // parent.
            EXIT_PROG = ((short) 0x8010), // exit program but leave it in
                                          // process queue
            STOP_PROG = ((short) 0x8011),
            FETCH_GLOBAL = ((short) 0x8012),
            STORE_GLOBAL = ((short) 0x8013),
            FETCH_EXTERNAL = ((short) 0x8014),
            STORE_EXTERNAL = ((short) 0x8015),
            EXPORT_VAR = ((short) 0x8016),
            EXPORT_PROC = ((short) 0x8017),
            SWAP = ((short) 0x8018),
            SWAPA = ((short) 0x8019),
            POP = ((short) 0x801A),
            DUP = ((short) 0x801B),
            POP_RETURN = ((short) 0x801C),
            POP_EXIT = ((short) 0x801D),
            POP_ADDRESS = ((short) 0x801E),
            POP_FLAGS = ((short) 0x801F),
            POP_FLAGS_RETURN = ((short) 0x8020),
            POP_FLAGS_EXIT = ((short) 0x8021),
            POP_FLAGS_RETURN_EXTERN = ((short) 0x8022),
            POP_FLAGS_EXIT_EXTERN = ((short) 0x8023),
            POP_FLAGS_RETURN_VAL_EXTERN = ((short) 0x8024),

            // return from a procedure called from C, leaving the return value
            // on the stack.
            POP_FLAGS_RETURN_VAL_EXIT = ((short) 0x8025),
            POP_FLAGS_RETURN_VAL_EXIT_EXTERN = ((short) 0x8026),

            CHECK_ARG_COUNT = ((short) 0x8027), // call to do a run-time check
                                                // of arguments to functions
            LOOKUP_STRING_PROC = ((short) 0x8028), // call to lookup a procedure
                                                   // index given a string
            POP_BASE = ((short) 0x8029), POP_TO_BASE = ((short) 0x802A), PUSH_BASE = ((short) 0x802B),
            SET_GLOBAL = ((short) 0x802C), FETCH_PROC_ADDRESS = ((short) 0x802D), DUMP = ((short) 0x802E),
            IF = ((short) 0x802F), WHILE = ((short) 0x8030), STORE = ((short) 0x8031), FETCH = ((short) 0x8032),
            EQUAL = ((short) 0x8033), NOT_EQUAL = ((short) 0x8034), LESS_EQUAL = ((short) 0x8035),
            GREATER_EQUAL = ((short) 0x8036), LESS = ((short) 0x8037), GREATER = ((short) 0x8038),
            ADD = ((short) 0x8039), SUB = ((short) 0x803A), MUL = ((short) 0x803B), DIV = ((short) 0x803C),
            MOD = ((short) 0x803D), AND = ((short) 0x803E), OR = ((short) 0x803F), BWAND = ((short) 0x8040),
            BWOR = ((short) 0x8041), BWXOR = ((short) 0x8042), BWNOT = ((short) 0x8043), FLOOR = ((short) 0x8044),
            NOT = ((short) 0x8045), NEGATE = ((short) 0x8046), WAIT = ((short) 0x8047), CANCEL = ((short) 0x8048),
            CANCELALL = ((short) 0x8049),

            STARTCRITICAL = ((short) 0x804A), ENDCRITICAL = ((short) 0x804B),

            END_CORE = ((short) 0x804C),

            // ////////////////////////////////////////////////////
            OPERATOR = ((short) 0x8000), INT = ((short) 0x4000), FLOAT = ((short) 0x2000), STRING = ((short) 0x1000),

            INTOP = ((short) (CONST | INT)), FLOATOP = ((short) (CONST | FLOAT)),
            STRINGOP = ((short) (CONST | STRING));
}
// @formatter:on
