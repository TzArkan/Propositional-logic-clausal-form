This C++ program implements a basic logic reasoning system 
using resolution in Propositional Logic, 
specifically in Clausal Form 
(FNC - Forma Normală Conjunctivă, i.e., CNF: Conjunctive Normal Form). 
Here's a brief overview of what it does:

To check if a set of propositional logic formulas leads to a contradiction, i.e., if the system is inconsistent.

Input Parsing:

The program starts with a comma-separated string of logical implications (e.g., "p->q,q->r,r->s,s->t").

These are parsed into individual formulas.

Conversion to CNF (FNC):

Each formula of the form a->b is converted to CNF: !a V b (represented as naVb).

Negations like !(a->b) are interpreted and converted to a conjunction of a and !b.

Resolution Loop:

The program attempts to apply the resolution rule to all pairs of clauses:

If two clauses contain complementary literals (e.g., a and na), they can be resolved to produce a new clause.

The new clauses (from resolution) are added to the current set, and the process repeats until no new clauses can be generated.

Contradiction Check:

If at any point a literal and its negation both exist in the set (e.g., p and np), a contradiction is detected, and the system is declared inconsistent.