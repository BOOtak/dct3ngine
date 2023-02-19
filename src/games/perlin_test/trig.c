#include "trig.h"
#include "fixed.h"

#define correct_angle(a) ((long long)a * ONE / (1 << 30))

static const Fixed costable[] = {
    correct_angle(1073741824),
    correct_angle(1073578288),
    correct_angle(1073087729),
    correct_angle(1072270298),
    correct_angle(1071126243),
    correct_angle(1069655912),
    correct_angle(1067859754),
    correct_angle(1065738315),
    correct_angle(1063292242),
    correct_angle(1060522280),
    correct_angle(1057429273),
    correct_angle(1054014162),
    correct_angle(1050277989),
    correct_angle(1046221891),
    correct_angle(1041847103),
    correct_angle(1037154959),
    correct_angle(1032146887),
    correct_angle(1026824413),
    correct_angle(1021189159),
    correct_angle(1015242840),
    correct_angle(1008987269),
    correct_angle(1002424350),
    correct_angle(995556083),
    correct_angle(988384560),
    correct_angle(980911966),
    correct_angle(973140576),
    correct_angle(965072759),
    correct_angle(956710970),
    correct_angle(948057759),
    correct_angle(939115760),
    correct_angle(929887697),
    correct_angle(920376381),
    correct_angle(910584710),
    correct_angle(900515665),
    correct_angle(890172315),
    correct_angle(879557810),
    correct_angle(868675383),
    correct_angle(857528349),
    correct_angle(846120104),
    correct_angle(834454122),
    correct_angle(822533958),
    correct_angle(810363241),
    correct_angle(797945680),
    correct_angle(785285058),
    correct_angle(772385229),
    correct_angle(759250125),
    correct_angle(745883746),
    correct_angle(732290163),
    correct_angle(718473518),
    correct_angle(704438018),
    correct_angle(690187940),
    correct_angle(675727625),
    correct_angle(661061475),
    correct_angle(646193961),
    correct_angle(631129609),
    correct_angle(615873009),
    correct_angle(600428808),
    correct_angle(584801711),
    correct_angle(568996477),
    correct_angle(553017922),
    correct_angle(536870912),
    correct_angle(520560366),
    correct_angle(504091252),
    correct_angle(487468587),
    correct_angle(470697435),
    correct_angle(453782903),
    correct_angle(436730145),
    correct_angle(419544355),
    correct_angle(402230767),
    correct_angle(384794656),
    correct_angle(367241333),
    correct_angle(349576144),
    correct_angle(331804471),
    correct_angle(313931728),
    correct_angle(295963357),
    correct_angle(277904834),
    correct_angle(259761657),
    correct_angle(241539355),
    correct_angle(223243478),
    correct_angle(204879599),
    correct_angle(186453311),
    correct_angle(167970228),
    correct_angle(149435979),
    correct_angle(130856211),
    correct_angle(112236583),
    correct_angle(93582766),
    correct_angle(74900443),
    correct_angle(56195305),
    correct_angle(37473049),
    correct_angle(18739379),
    correct_angle(0),
};

Fixed fix_cos(Fixed deg)
{
    int sign = 0;
    Fixed val;
    int i;

    if (deg < FIX(0))
        deg = -deg;
    while (deg >= FIX(360))
        deg -= FIX(360);
    if (FIX(180) - deg <= 0)
        deg = FIX(180) + (FIX(180) - deg);
    if (FIX(90) - deg <= 0)
    {
        deg = FIX(90) + (FIX(90) - deg);
        sign = 1;
    }

    if (deg <= FIX(0))
        val = ONE;
    else if (FIX(90) - deg <= 0)
        val = 0;
    else
    {
        i = deg / ONE;
        val = deg % ONE;
        if (val != 0)
            val = (costable[i] * (ONE - val) + costable[i + 1] * val) / ONE;
        else
            val = costable[i];
    }

    return !sign ? val : -val;
}

Fixed fix_sin(Fixed deg)
{
    return fix_cos(FIX(90) - deg);
}
