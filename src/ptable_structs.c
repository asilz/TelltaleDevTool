struct PhonemeTable__PhonemeEntry
{
    struct AnimOrChore animation;
    float contributionScalar;
    float timeScalar;
};

struct PhonemeTable
{
    struct String name;
    float contributionScale;
    void *animations; // Map<Symbol,PhonemeTable::PhonemeEntry,std::less<Symbol>_>
};