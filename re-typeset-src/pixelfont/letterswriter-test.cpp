#include <gmock/gmock.h>
#include <climits>
#include <QVector>
#include "letterswriter.hpp"
#include <QDebug>

class PaintDeviceMock : public LettersWriter::PaintDevice {
public:
    PaintDeviceMock()
        : pa_(Letters::NumLines)
    {
        //NOOP
    }

    virtual void setPixel(const QPoint &position, uint index_or_rgb) {
        if( pa_[position.y()][position.x()].unicode() == '#' ) {
            pa_[position.y()][position.x()] = '+';
        } else {
            pa_[position.y()][position.x()] = '-';
        }
    }
    virtual int width() const {
        return pa_[0].size();
    }
    virtual int height() const {
        return pa_.size();
    }
    QVector<QString> pa_;
    bool check() {
        for( int y=0; y<pa_.size(); ++y ) {
            for( int x=0; x<pa_[y].size(); ++x ) {
                if( pa_[y][x].unicode() != '+' && pa_[y][x].unicode() != ' ' ) {
                    return false;
                }
            }
        }
        return true;
    }
};


class LettersWriterTest : public ::testing::Test {
public:
    LettersWriterTest()
        : lw(pdm)
    {
        //NOOP
    }

    PaintDeviceMock pdm;
    LettersWriter lw;
};


TEST_F(LettersWriterTest, calculateLength)
{
    EXPECT_EQ( 0, LettersWriter::calculateLength("") );
    EXPECT_EQ( 3, LettersWriter::calculateLength("p") );
    EXPECT_EQ( 7, LettersWriter::calculateLength("pp") );
    EXPECT_EQ( 5+5+5+2, LettersWriter::calculateLength("M M") );
}

TEST_F(LettersWriterTest, write_M)
{
    pdm.pa_[0]="#   #";
    pdm.pa_[1]="## ##";
    pdm.pa_[2]="# # #";
    pdm.pa_[3]="#   #";
    pdm.pa_[4]="#   #";

    QRect area( 0, 0, pdm.pa_[0].size(), pdm.pa_.size() );
    lw.write("M", area, 1);
    EXPECT_TRUE( pdm.check() );
}
TEST_F(LettersWriterTest, write_M_centered)
{
    pdm.pa_[0]="  #   #  ";
    pdm.pa_[1]="  ## ##  ";
    pdm.pa_[2]="  # # #  ";
    pdm.pa_[3]="  #   #  ";
    pdm.pa_[4]="  #   #  ";

    QRect area( 0, 0, pdm.pa_[0].size(), pdm.pa_.size() );
    lw.write("M", area, 1);
    EXPECT_TRUE( pdm.check() );
}
TEST_F(LettersWriterTest, write_M_centered_2)
{
    pdm.pa_[0]=" #   #  ";
    pdm.pa_[1]=" ## ##  ";
    pdm.pa_[2]=" # # #  ";
    pdm.pa_[3]=" #   #  ";
    pdm.pa_[4]=" #   #  ";

    QRect area( 0, 0, pdm.pa_[0].size(), pdm.pa_.size() );
    lw.write("M", area, 1);
    EXPECT_TRUE( pdm.check() );
}
TEST_F(LettersWriterTest, write_M_centered_wrong)
{
    pdm.pa_[0]="  #   #";
    pdm.pa_[1]="  ## ##";
    pdm.pa_[2]="  # # #";
    pdm.pa_[3]="  #   #";
    pdm.pa_[4]="  #   #";

    QRect area( 0, 0, pdm.pa_[0].size(), pdm.pa_.size() );
    lw.write("M", area, 1);
    EXPECT_FALSE( pdm.check() );
}
TEST_F(LettersWriterTest, write_M_centered_wrong_2)
{
    pdm.pa_[0]="#   #  ";
    pdm.pa_[1]="## ##  ";
    pdm.pa_[2]="# # #  ";
    pdm.pa_[3]="#   #  ";
    pdm.pa_[4]="#   #  ";

    QRect area( 0, 0, pdm.pa_[0].size(), pdm.pa_.size() );
    lw.write("M", area, 1);
    EXPECT_FALSE( pdm.check() );
}
TEST_F(LettersWriterTest, write_oo)
{
    pdm.pa_[0]="       ";
    pdm.pa_[1]="       ";
    pdm.pa_[2]=" #   # ";
    pdm.pa_[3]="# # # #";
    pdm.pa_[4]=" #   # ";

    QRect area( 0, 0, pdm.pa_[0].size(), pdm.pa_.size() );
    lw.write("oo", area, 1);
    EXPECT_TRUE( pdm.check() );
}
TEST_F(LettersWriterTest, write_i_j)
{
    pdm.pa_[0]="#        #";
    pdm.pa_[1]="          ";
    pdm.pa_[2]="#        #";
    pdm.pa_[3]="#        #";
    pdm.pa_[4]="#       # ";

    QRect area( 0, 0, pdm.pa_[0].size(), pdm.pa_.size() );
    lw.write("i j", area, 1);
    EXPECT_TRUE( pdm.check() );
}
TEST_F(LettersWriterTest, writeH_B)
{
    pdm.pa_.resize(4);
    pdm.pa_[0]=" # # ";
    pdm.pa_[1]="# # #";
    pdm.pa_[2]="# # #";
    pdm.pa_[3]="#####";

    QRect area( 0, 0, pdm.pa_[0].size(), pdm.pa_.size() );
    lw.write("B", area, 1, LettersWriter::Vertical);
    EXPECT_TRUE( pdm.check() );
}
TEST_F(LettersWriterTest, writeH_a_BB)
{
    pdm.pa_.resize(20);
    pdm.pa_[ 0]=" # # ";
    pdm.pa_[ 1]="# # #";
    pdm.pa_[ 2]="# # #";
    pdm.pa_[ 3]="#####";
    pdm.pa_[ 4]="     ";
    pdm.pa_[ 5]=" # # ";
    pdm.pa_[ 6]="# # #";
    pdm.pa_[ 7]="# # #";
    pdm.pa_[ 8]="#####";
    pdm.pa_[ 9]="     ";
    pdm.pa_[10]="     ";
    pdm.pa_[11]="     ";
    pdm.pa_[12]="     ";
    pdm.pa_[13]="     ";
    pdm.pa_[14]="     ";
    pdm.pa_[15]="     ";
    pdm.pa_[16]="    #";
    pdm.pa_[17]="  ###";
    pdm.pa_[18]="  # #";
    pdm.pa_[19]="   # ";

    QRect area( 0, 0, pdm.pa_[0].size(), pdm.pa_.size() );
    lw.write("a BB", area, 1, LettersWriter::Vertical);
    EXPECT_TRUE( pdm.check() );
}
TEST_F(LettersWriterTest, writeH_B_centered)
{
    pdm.pa_.resize(8);
    pdm.pa_[0]="     ";
    pdm.pa_[1]="     ";
    pdm.pa_[2]=" # # ";
    pdm.pa_[3]="# # #";
    pdm.pa_[4]="# # #";
    pdm.pa_[5]="#####";
    pdm.pa_[6]="     ";
    pdm.pa_[7]="     ";

    QRect area( 0, 0, pdm.pa_[0].size(), pdm.pa_.size() );
    lw.write("B", area, 1, LettersWriter::Vertical);
    EXPECT_TRUE( pdm.check() );
}
