using namespace com::sun::star::uno;
using namespace com::sun::star::frame;

void CreateNewDocument(Reference< XFrame > &rxFrame, int keyboard_layout,  int globalWordCnt, int globalWordLenght);
void MakeRed(Reference< XFrame > &rxFrame );
void MakeStatistics (Reference< XFrame > &mxFrame );
bool MyIsAlpha(char16_t letter);
bool IsOnlyLatin(rtl::OUString curWord);
bool MyIsAlphaAndDigit(char16_t letter);