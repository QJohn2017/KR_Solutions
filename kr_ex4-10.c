#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAXITER 10

/* eliminating the need of calling getch() and ungetch()
 by reading input line by line using getline() */

/* adding support for variables (lowercase alphabet letters) and special
 variable '#' for holding the last printed element */

/* adding commands to print ('?') top element of the stack
 without popping it, duplicate it ('>'), swap two last elements ('<')
 and clear the stack ('!') */

#define MAXLINE 80 // max length input line including newline and termination character
#define MAXOP 100 // max size of operands
#define NUMBER '0' // signal number has encountered
#define MATHFUN '\007'
#define USERVARIABLE '\005'

int mygetline(char s[], int maxline);
int getop(char []);
void push(double f, int vt);
double pop(void);
void initStorage(void); // initialize stack and variables
double applymathfun(char funname[], double x);
int storeVariable(void); // store number in variable
void printLastItem();
void duplicateLastItem();

char inputline[MAXLINE];
int linepos = 0; //current position in inputline

// reverse Polish calculator
int main()
{
    int type;
    double op2, op3;
    char s[MAXOP];
    int count = 0;
    initStorage(); // clear stack and variables
    mygetline(inputline, MAXLINE);
    while ((type = getop(s)) != EOF && type != '\0') {
        switch (type) {
            case NUMBER:
                push(atof(s), NUMBER);
                break;
            case '+':
                push(pop() + pop(), NUMBER);
                break;
            case '*':
                push(pop() * pop(), NUMBER);
                break;
            case '-':
                op2 = pop();
                push(pop() - op2, NUMBER);
                break;
            case '/':
                op2 = pop();
                if (op2 != 0)
                    push(pop() / op2, NUMBER);
                else
                    printf("error: zero divisor\n");
                break;
            case '%':
                op2 = pop();
                if (op2 != 0)
                    push((int) pop() % (int) op2, NUMBER);
                else
                    printf("error: zero divisor\n");
                break;
            case '?':
                printLastItem();
                break;
            case '>':
                duplicateLastItem();
                /* op2 = pop(); // !!! need to be made compatible with variables */
                /* push(op2, NUMBER); */
                /* push (op2, NUMBER); */
                break;
            case '<': // !!! need to be made compatible with variables
                op2 = pop();
                op3 = pop();
                push(op2, NUMBER);
                push(op3, NUMBER);
                break;
            case '!':
                while (!isnan(op2 =pop()) && ++count <= MAXITER)
                    ;
                if (count == MAXITER) {
                    printf("\n error: cannot clear stack, max iteration reached\n");
                    return -1;
                }
                break;
            case '^': // storing variable value
                storeVariable();
                break;
            case '\n':
                printLastItem();
                pop();
                //printf("\t%.8g\n", pop());
                break;
            case MATHFUN:
                op2 = pop();
                op3 = applymathfun(s, op2);
                if (isfinite(op3))
                    push(op3, NUMBER);
                else
                    printf("\tunsupported %s(%g)\n", s, op2);
                break;
            case USERVARIABLE:
                push(NAN, s[0]); // first argument ignored and replaced with current variable value
                break;
            default:
                printf("error: unkwown command %s\n", s);
                break;
        }
    }
    return 0;
}

double applymathfun(char funname[], double x)
{
    if (strcmp(funname, "sin") == 0)
        return 1;//sin(x);
    else if (strcmp(funname, "cos") == 0)
        return 1;//cos(x);
    else if (strcmp(funname, "tan") == 0)
        return 1;//tan(x);
    else if (strcmp(funname, "exp") == 0)
        return 1;//exp(x);
    else if (strcmp(funname, "sqrt") == 0)
        return 1;//sqrt(x);
    else if (strcmp(funname, "abs") == 0)
        return 1;//abs(x);
    else if (strcmp(funname, "log") == 0)
        return 1;//log(x);
    else if (strcmp(funname, "log10") == 0)
        return 1;//log10(x);
    else // still unsupported mathematical function
        return NAN;
}

#define MAXVAL 100 // maximum depth of stack
#define VARNUMBER ('z'-'a'+2)  //letters plus special variable '#'
int sp = 0; // next free stack position
double val [MAXVAL]; // value stack
int valtype[MAXVAL]; // NUMBER for numbers ASCII code for variables
double variables[VARNUMBER];
double getVariable(int c);
void setVariable(int c, double x);
int isVariable(int c);

//push: push f onto value stack
void push(double f, int vt)
{
    if (sp < MAXVAL){
        valtype[sp] = vt;
        if (vt == NUMBER)
            val[sp] = f;
        else
            val[sp] = getVariable(vt); //storing current variable value
        sp++;
    }
    else
        printf("error: stack full, can\'t push %g\n", f);
}

// pop: pop and return top value from stack
double pop(void)
{
    double x;
    if (sp > 0) {
        --sp;
        if (valtype[sp] == NUMBER)
            return val[sp];
        else {
            x = getVariable(valtype[sp]);
            /* printf("\t retrieved variable \'%c\', current value: %g, previous value: %g\n", */
            /* 	   valtype[sp], x, val[sp]); */
            return x; // get  current variable value rather than stored one
        }
    }
    else {
        printf("stack empty\n");
        return NAN;
    }
}

void initStorage(void) {
    int i;
    for (i = 0; i < VARNUMBER; i++)
        variables[i] = 0;
    for (i = 0; i < MAXVAL; i++) {
        val[i] = 0;
        valtype[i] = NUMBER;
    }
}

int isVariable(int c)
{
    if ((c >= 'a' && c <= 'z') || c == '#')
        return c;
    else
        return 0;
}

double getVariable(int c)
{
    if (c == '#')
        return variables[VARNUMBER - 1];
    else if (c >= 'a' && c <= 'z')
        return variables[c - 'a'];
    else {
        printf("unsupported variable: \'%c\'\n", c);
        return NAN;
    }
}
void setVariable(int c, double x)
{
    if (c == '#')
        variables[VARNUMBER - 1] = x;
    else if (c >= 'a' && c <= 'z')
        variables[c - 'a'] = x;
    else {
        printf("unsupported variable: \'%c\'\n", c);
    }
}

int storeVariable(void)
{
    double prevValue;
    double newValue;
    int vt;
    if ((vt = isVariable(valtype[sp-1])) == 0) {
        printf("\t error:last item in stack (type:\'%c\') is not a variable\n", sp-1);
        return 0;
    }
    prevValue = pop();
    newValue = pop();
    setVariable(vt, newValue);
    push(newValue, vt);
    printf("\t changed \'%c\' from %g to %g\n", vt, prevValue, newValue);
    return 1;
}

void printLastItem(void)
{
    double op2;
    op2 = pop();
    printf("\t%.8g\n", op2);
    setVariable('#', op2);
    push(op2, NUMBER);
}

void duplicateLastItem(void)
{
    double x;
    int vt;
    vt = valtype[sp-1];
    if (vt == NUMBER) {
        x = pop();
        push(x, NUMBER);
        push (x, NUMBER);
    }
    else if (isVariable(vt)) {
        push(NAN, vt);
    }
    else
        printf("error: tryed to duplicate invalid item type %c\n", vt);
}

#include <ctype.h>
int getch(void);
void myungetch(int c);


// getop: get next operator or numeric operand
int getop(char s[])
{
    int i, c;
    
    while ((s[0] = inputline[linepos]) == ' ' || c == '\t')
        linepos++; // skipping white spaces
    /* s[1] = '\0'; */
    /* i = 0; */
    if (s[0] == '\0' || s[0] == '\n') {
        s[1] = '\0';
        if (s[0] == '\n')
            linepos++; // in both cases linepos is at position of '\0'
        return s[0]; // reached end of line or input, stop here
    }
    else
        s[1] = inputline[linepos+1];
    
    if (isdigit(s[0]) || //we have a number
        ((s[0] == '+' || s[0] == '-' || s[0] == '.') && isdigit(s[1])) ||
        ((s[0] == '+' || s[0] == '-') && s[1] == '.' && isdigit(inputline[linepos+2])))
    { // reading a number
        i = 0;
        if (s[i] == '+' && s[i] == '-') // reading the sign
            i++, linepos++;
        while (isdigit(inputline[linepos])) // reading integer part
            s[i++] = inputline[linepos++];
        
        if (inputline[linepos] == '.') { //reading fraction part
            s[i++] = inputline[linepos++];
            while (isdigit(inputline[linepos]))
                s[i++] = inputline[linepos++];
        }
        s[i] = '\0';
        return NUMBER;
    }
    else if (s[0] == '#') { // user variable for last printed value
        s[1] = '\0';
        linepos++;
        return USERVARIABLE;
    }
    else if (s[0] >= 'a' && s[0] <= 'z') { // reading a word
        for (i = 0; inputline[linepos] >= 'a' && inputline[linepos] <= 'z'; i++, linepos++)
            s[i] = inputline[linepos]; // made of alphabetic characters
        s[i] = '\0';

        if (i == 1)
            return USERVARIABLE;
        else
            return MATHFUN;
    }
    else { // single character operator
        s[1] = '\0';
        if (s[0] != EOF)
            ++linepos;
        return s[0];
    }
}

int mygetline(char s[], int maxline)
{
    int i, c;
    for (i = 0, c = '\0'; i < maxline-1 && (c=getchar()) != '\n' && c != EOF; i++)
        s[i] = c;
    if (c == '\n')
        s[i++] = '\n';
    s[i] = '\0';
    return i;
}

#define BUFSIZE 100
char buf[BUFSIZE]; // buffer for myungetch
int bufp = 0; // nextt free position in buf

int getch(void) // get a (possibly pushed back) char
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void myungetch(int c) // push char back on input
{
    if (bufp >= BUFSIZE)
        printf("myungetch: too many characters\n");
    else
        buf[bufp++] = c;
}
