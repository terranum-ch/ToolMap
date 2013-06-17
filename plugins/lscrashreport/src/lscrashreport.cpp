/***************************************************************************
 lscrashreport.cpp
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber
 email                : lucien.schreiber at crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "lscrashreport.h"
#include "lscrashreport_param.h"

void lsCrashReport_DLG::OnLinkClicked(wxHyperlinkEvent & event) {
    //wxLogWarning(_("Not implemented :-P directory is: %s"), m_CrashDirectory);
    wxLaunchDefaultApplication(m_CrashDirectory);
}



void lsCrashReport_DLG::_CreateBitmaps(){
    {
		wxMemoryInputStream sm("\211PNG\r\n\032\n\000\000\000\rIHDR\000\000\000@\000\000\000@\b\006\000\000\000\252iq\336\000\000\000\011pHYs\000\000\013\023\000\000\013\023\001\000\232\234\030\000\000\030\332IDATx\001\355Zi\224]U\225\336w|\363\253\271R\231\213Ld0$\020BHBb *aH\032\305\320\"\n\342\320\3324\255\355\262\245\273\225^\035\034\020\333\345\037[\324n\305\205\210\335HZ\264e2D$a0\020LB\022*S\245R\363\\\365\352\315\357\316\267\277}^\275P\224\251JR\011,\177\344$\247\356\275\347\236{\356\331\337\376\366>{\237\373\210.\224\013\b\\@\340l\020X7%V\275rJl\325\331<\363\227\334W>\333\311\271\232\253\311\212\277\001\317\235\365\263g\373\256w\243\377Y\013\361Rk\276[\226\311^3-~\371\2731\301w\372\035g\r\000OH\225\325\375\201\240\362\225e\313H{\247'x.\343/\257\211\324]15V5\336\030\352x7\307\272\247\313\332!- ]3\303\233\374\251=\324\375\243\261\372\215\327~\373U\263f\004#\241\331\201@\240\302\266\314\234\221/\234x\370\305\023Mx\306\033\357\271\263\270\247jA\351n\337\245G\361\314\340X\317Ic\335\030\257}\321\"\322/\326\246\274\240\352Z8\223\320\256|\366\370qs\274\376\245{\313*\250\254\262\272\372\372XD\277%\034\r\256\b\006\203u\341P@2-\233\nyc \2333\032\362\271\302\023\251\\\376W\273:\n\235\245\347&r\274jZ\354\237\362\216\343\355\355)|g\274\347\225\361n\216u\257\277\237\334\213*B\355\341H\340\316HX\332z\240c(1V_n\337\262\205\344\374!\375&M\327\037\3224\345n\324\371\262,\305H\"\311\266\035\262m\233k\330\261\335z\\_\347z\376\207g\306twj\334:\330\221&g\274\261G\337[\267\216\324\367Dj\356+\330\356_\265\246r\367\344,\312\215\3563\362zB\000\360\000\215\003\271\356\367L-\277\315\364\234\201\303\235\351?\215\034t\364y\362H\371\314[o\275\361Y\317\363/\352\356\356#\327uH\222\340~|\237\034\307!\313r\3100\301\002\323\"\303pH\226\2252-\240^\347y\312\376\266\224uh\364x\343\\K+jg<\200\373\377\3227T\370f\323\220\275c\234\276\342\326\204\001\300\323\316\274\272\370\325\022\371\313d;\375D\177\236\254S\275l\355\314\370\365\351\202\361\321\303-=+\347\324OV\027/\236K\241H\224\262\331,es&\204\207\366\035\017\014\360\310\361|\362e\205lP\243?cR\322ps\327\255Y<T\325\324\323\331r\006\276\341\216\265s\327\372\236\367\237]\211\354\363\a;2\337\005uR\247\232\323\310\266s\001\200.\256\013W\204\003\201\317O\251\255\276a\311\354\252}\aZ\022#\355VZ==\362EI\222\036\212\006\324\325N\241\2406\034k\245\023\035\203\024+\213\323\344\351S\250\246\"B\345\345Q\352O\025(g\272\2241\\\262\000B0\022\241\272I\3254}\346\314\245R\260\354v\245\256|R\264\265\347\205n\0026c\224\315\233\027\351\001\303\177\250k0\333\375r\343\340\3035D\257g\306\351_\032\346\234\000\250\213I\255\210\207\026\313$-\363\034\367\332yu\221\201\313fLj=\330\231\240\025\265\372\2674]\273O\362I\361\3616]\223)\036\322qaSGG\017\035m\004\030\335C\324\332\237\245\376D\226\206\362&\305\243!\232<\245\206\302\3612r$\235\\\260AR\0249\022\257\270<:\265v~\225\233\336\336\2262\215\322\344\371\270e\313\026y\236\332?]\31797\231\246\365\261\235\207{\036\005\225^\033\"j\031\331o\254\363\011\255\002#\aC \260\242.\036Xv\345\334\232\270\3458\177\223\317\347\315\245k7u\016\364\365\254?\264{'\351\201\340\310\356\342\\\302[\341\377\310\363<v\003Tp}J\271\022\205\202\001R5\215\202A\0355D\201`\220\002\201\020\351\301\2600\033#\233\336\236\352k\276C\357nMQ\305\224\352`H\256\323u\355\357\025E\331\204\201\364\266\336\324\257^j\034x|>\321\357\032\350\324&9z2\347\304\000\036,\032\240p2\357^t\260;\375{S\221~n\246\215\331\213W\277\357\246M\037\277\233\366\276\364\034e\323I\362\\\227<\226\224\013\244\347\177\342\n\3472*\333~\332rI\222%\004YhC\250\211U\002\367pD\345v\206,\034+\237\235\352\356\372`\336\361>\247\353\312\335\350\266\001\343\224\371\236\177\004\276\344\271W\216\366\355\r\253\376\236V\227z\305\273\316\340\3179\0030\247\222\254\244I\363\035\237.\273%j\325\377l\231\247=\272\363\300\374\031\253\326\207>\371\205{\250v\312t\252\254\251\241xy\031\005C!\222\025\304\321\226\005\205\025\001a\321,0\201\355\237\231\301\200\300o\234\004\200\205\347kUU)\221HP\240\220\252\230\025\0176v\346\314\317\353\262\364\355%v\345\203\217\355k\334~\250;\335\354I~\355\364\n\3325\322!o&R\312/*\237\321\2364\340\022\212\270\217\304\345\234\001\340\227A\370\346\317L\243\330\327\226\251\337\2354I\271\346\375\325F\350\311\337n\247\310\245\357\245\315\037\273\235\336{\335&Z\267\341&\252\237=\233\206\372\273)\231L\220\2217\204\300\014\200\r\023H\024\330\277\025\005W\206\205\226\230\011`\000(N\331\202A\321\334\020\335\377\361\215tu\177\223\2658\223z\370\336\206d\307\033\375\375\013\3222m\302\020\213\202\n\035j\317\322\341\221\002\306\246\206\246\302\011\335S\233\266\266\303\211\376Y\224y\022\200\325\325\024\253\257\014-\235\023\327\2509\3550Zg\\\374\315\244n\\\022\375\367hM`\261\245\253\024\n\253ty<K\017>\374[Rf\314\243y\363\027\011{\256\254\251%\273\220\241\216\326\023\224\313 \302A\014\300\032v0\373\376\234\r\365\300cBhn\023L\340#\256\013\216K\321L\212\036\370\370&\322\377\370{*\014\016V\030\262\374\201\335\203\226\333\341\372u\272L\331\260L\355p\033\273G\a>\223\242\372G\024\227\332vg\355}\247\022\350$\000\323*C5\276$\177\332r\245\273gU\aW.\236^\351\255\230[Qv\305\274*wosr\334h\352\336\r\325\367\312U\221O\272!8<]'\0374\017\206\024\272$\222\243\373\037\331F\223\027-\245\231\027\315\201C\014\0113H\364vPOg\a\302\337\202`\001\302\000\352\311X\360\0230\001\304G\314\000\006\000\013\000\345M\207\242\256M\017\334r-\205_{\221r\275\320#\234d\271*W,\211)\366/{\315o\324\373\264\267\323\247\206/\217\212\372n^6\345\001\311\363?\333\227\266\2766`8\311S\001\300\014|[\251\014\321\264\351\261\360\215\223\252\302\037\255\251\210-\013G\302\275\321H\350e8\252\326\\6[]\310\031\345\271\234\361j\266P8\326\235\265\207\356[\035\237\265yy\374Gp\3331\327\207sCh\353[X\251\260Zi\216AG\333S\264\345OA\272\362\023_\246;?\367w\360\005\325\364\307m\277\246\307\177\372 \035|\343\r\262\r\213L\004A\azs\004S\242\230\256P8\240\2021\032\031\266K\213\246\325\3217\256_E\345\273wQ!\225 \265\014~\004}|\337C\034\355\0329\313\272y\371+C\317\260\020w\255[\024\265\025w\212\241\330I/O\367\230y\343K\003C\271\247v4'?\210\333\247\014\251O2\240\204B\301\241to\316\376S\323@\376\267!\331oi\037\310 v\311\034\b\353rV\221\3502\323r6Z\216{\225i\271\263d\217\356\270\371\352K>5c\301\234 \274\033\221\006\006(*\201I\254F\3625\205j+$\332P\227\246W\237\335A?\331\3724y\201\000]r\371\n\021\017\2646\035\247L\n\246\000\325\367fm\001\200\003\347\210h\216\342\030\343\266\325\227\322WW.\"\345\371?\220\231\317\220\032\017\223\n`\224\260N2X\240\"/\357\265\274\365\255\244lX05v\207\353{\377\0149\356!\227\356\264,w\345\321\216\241\237\"\231\370\037x\227\243%\371F\037\377\214\001\243;\340z\001\304Y\013\226\232!\242\303K\352\343\004\233K\014\206\322\255\337\337\264~\323\232\325+\037Sd\033^\252\213<+O~!\215\232\002\013@o\317\"\311I\223b\364\023\345\035:\216d\367\221\003Dm\223\026\322\364\345+(\323\337I\r\273_\247>D\202G\006\r\214+\321\334\262 ]\277p\006}ha=\325\016%i\350\300\233\344C\343Z,D!\254\271JH#'\200\350\003\276\206\343\b\3252\351\241\343\311];2\336\216\252\260\036RTe*4P\3231\230\335\366\032\242\246\000Q\003R\325\347N!\227h:\023\000hN\224j\206L\2527}\n\005d\252\312\271\024\307\252\225\350\276\367\222O\324-Y\372!Rt\370W\233<\a\216,?$\252\214UGv\r\000\221#\337\034D\265I\206eH\260\304\246V\242'[\210~\223\013\320Q\313G\022\344\320\212\210L\037\236[E\253\246VQY6O\271\216^\221$I\341\000\311`\215\251\312\364HO\036\004S\350\316E\360\330ea\000\243\221\346\271$\333\3463\223\177~\364\006H\204\211\320\314r\215\242i\233\256\014(\324\253\271\264#M4f\266\372g& `\031\365'aQ\276\340Q\227\345Qk\225K\255\0318\353k\252i\315\355\363\274O\352\272\001U\230\240\255K\276\213\365\035\232\227\301\204L*O\235\260\353\250\223'\325\306}\333G\026\310k>\354\034o]\036#\272\006\356\371\315~\227Vi>\335?\211h>\207\372\235\275d\244\301\"\204\3162kZRH\207C|\020\353\333\367;\n\264+aR:o\321\372\311Q\222`b\204j\273\336\314T\316p\017$\254\005\320\350\345\230\353\305\320|zv5\375_G~\374\204\350\214\000\030\211\a\320d\247\335\371\362\265\332\222\252*\365F\0170\373\240\256\217\024\027\036\r\302\347(9\224\247\037\274\220\243\237\356\316\322\266#yZY\216e\0143\363\320\005;4\304]\315\002Q\b\347\027#\207\274\004\256\243L\366\311d\307\006\212I\032\374\210\017\037\342\361\270h\207\223\374~\217I=\360\2220G:\226\261\351\252J\215\246\306\002\310\027\020!j\222\"\273\316\320\3776g\267\301L9\0034b:\275\326\232\241\256\221s?\325\371\204\266\304x\240\032M\272\n1.\250\r^\313\200E\206D\274\214\301\024\216\266\031\324\320]\240d\301\242\337\365:\2644L\364\245%\"\375\207PE\0208\020tq\216\377\305\n\200D\324\307`\302)\372\234\0363k\000\002\aH \211(\014@\001\347\373\a\n\264|2\320\003p\036\034\360\232\231\345u\376\346\356\235\264\225\370\011u\320\032;s,\216\364\326x#\257\317\350\334_Aqlj\\N\310\345)\227'BPC\031\0307\327l\212\234\\\206\332\222\005jI[\"#\351\343(\002\325Cx\3451N\274s\300@\260\364\\0c\254\240\"&\340\020\231\231\342\3027x\014\002lF\001 W\303Gp\301-\n\240\357\234\260\202@\212\315\316%\027\3178\222t\351\317\373\253/\345\241\320\205\303\3123*\023b\300\367\314\340-\237\365\215\251>vp\240r\360\031~\026\313^Q(\027\006\350\220\002\236\267\303\030#\230\3215\025E\301\231\372\036\336(\264\016\372s\340\303\302s\031V\260\000\305\003\355]\326\276\203\214\0217Lh\371\3462\215\206\320\271\r\317\335T\027\240\345\225A\262@!\030\013\300\223\020\206(\221PP\277\002C\215\273;%^6\342\317Y\001\360\211\372\372\340\361@\356\336A?\377%8\031\335) \201a\217\240\000pL\002\206+l\274\026m\217b*\317\265\023\315B\307\325\345X\005A\024!8\004\362\030+\\\bs\200P\242\035\2222\026\354#<\330\272\270\3077\206\013g\211\377P\027\3042\250\220\2128\000\335\b\273\005\370\213\212\3601\200\340)\341\310\037\233\027\243\307\217eh\240\364\334\351\216E^\235\256\027\356\337\266f\316\302\\\215\373\244\246\251\327]\027\267\036\346\011\262v|\016\337\240i\312bJY\034s\240-\204\235\006\a\376\251\231D\353*\341\371A\020,\020\302\371\301m\260\353x\2532 \245\212\307\005\000\245\373h\027\205\261\305\011\326\0308J0\202\035\b\3328\336*\371\r\011Q\232\246++\343\261\310\2679\003\034~\362\264\207q\031\260e\335:\265'8\264\336s\235\325\226m\335e\313rc(\024\270\341\222\260{\227TQ\006\355ga\224\260CP\033\033=\220\244XY\b\3449\242\ra\274\020\2365\356b\302p\356\305\312\346\300} \213\a\366\210\364\030\347\014,/\227%\206H\230\341\260\236qRL\215O6\b\020\360\a\354\220\340\b\313\003r\002!\364\355\306\342\272\016:\330\363o\247\225\036\035\336\006\300\026\014\265wi\355\372\240\036\\\025\b\350\213;\224\236\032\305\223\327\362\254,\303~b\367\321\201\257\267$s\013_Y\025Y\375\376\331\363\310o9B~\016 \260\303baPYU%\r\263+\022v\317\002q;k\030\363e\234\234aF0\361qK\334\207\251\213g\005\013\270\023\227\222\320,$\013,*\237\017o\224\200\376\274\307 \203\001\325\001\245=\235\261\366\325\225\207\377\365#WN7\037{\265\375\376\342 c\377U\327\325S\260\340\204\257QI^\361\aMY\0214\274\253\014\253\320\246\026\254\203x\235\201\011o\303.\353\033G\2722\315\020`\025\206j\253\213ii6B\337\347\030\b6l\300f\031\000nc!\2075\310}Xh\006\204\227<!|\351\032\375\231\035\314\000\0377\270\037/`\334W\230\027\256\231\326,0\377\021GN\025\001\204\3208\337\303>\001\357\034\261\031p\033\200(;\326\237\337[Y\036\326\353\353\312\276\371\351\253\347G\364j\345[?\330\332\000-\235\272\250\031\271\"\240\373v$eX\215N\336\357t\\\351'-)k\000\363\253\301#\2250&\236Z>\256R\253\026\242F,i\275\323b\312\a\354\301\034Y)\314\026\213\262\207P\2005-@\021\202\024\037\022l\300\265`\004k\227+4\317\302r\177\301\016>\307\374=\b/1`\000\203\301*\331xQ`\\Bv\216\202\204\300\3208B\237\"@\303\3472\300( \223\302P\325;\033\a^\214\206\264\312i\223*\276\342\347\344\033>s\355\242\257\377\327\266\206'p\0173x{Q\367\234\030\342\310i\353\333\233\211\352\221ucg5T\2059\341\275\306q\207\314RBi$\354\224\036\314\221\215\200\333\317\002\000\2463&\217\027\024_\301Z\035>\027\014`\301q_\004>\014\b\013\315\302\002\004|,\021\221\013G/\022\220\340{\302\307\261\312\207\265+4\3144\347kh\236\351.\330q\022\b\334S%\344[B\r\310\032(\372\3737\273\237y\257\353\347\246M*\333\000\250\036\276c\355\234\273\376\332\260\237\316\344\362\377\375LC\177\017O\217\313\333|@\261\251\370\267\005B\343\314`tF\227|o\256M\307~\267\215\334\231\375 S\236\0133T\224\022\000,,\013?,\264`\302\360y\011\014\341\004\271M<\317Q \237\2402 8\262\360,0\011\241!\n\304\343\2552f\200\214\304HF.\300>\200\361Jf-\244U\330\217\341\224\303\243\334s\r=\277\253\357J\356\271xJ\271\027\r(1\214~\253*\313_x\377\234\252\257\226\a\006\037\337\332@\326\230\000\bA\306\370ce\334\346\254\222q1A\005y\216P7\346)\330Zz\244h\333\270b\341\030\204a\026\234\004\003\355\014\002b\036\034A{\034Y\253\302\336U_D{B\313\320\2547,\274\020\224\005\347D\211\201@e\020$TN\215;\022&\333:\2764R\016\202\235@\272`t\016\031\013Z\206z\220zQ_T\247\357\314\256\014\337\022\220\245\177L\033\321\017\257\231N\333&\004\200\211\254PFR\026\bJ\025b\273\233\265\206\302\006\310\202\227\212\000a\030\200\222c\023\0000\030\334\316\000\240b\017I,\221\274\n(\020\230s\332?\"\310\332\212\230b\0162\246\273\246\206\305\366\230\254\201\372%\301\261G\300\332\347\214PA[!o\372\215\375\371\024H\303*\311\200\011\375A\233\232\2031:\216\275\224\251\036>7 \220H\354\357\311\037@\210\022\235\034S\203uQ\255fB\0004vP\353\202zjF:^\201\371\027\275>\216\302S\263\020\014\0023\0027\021\321\n\355\226\004f&\b0pd\307\347B\365\330\032\025}\3709\035\317=\222r\350?\322.\005\001F/\372l\304\375\213q\3563\345\261\021\"\252\240?\316\005\000\022\r$\363\346\341\376\302\020\204C\nF\314\204\034RA\2232\204\260\014\221!hQ*|\332\006\210\270N\b\200[`c\373\034z\011\203^\306\373\200.\247\251\314^\014\314Z\334\207\034~\207\247P5\202\244\033e\244\302hc\212\277e\357\303\316N0\000\317\013\006\200\322x\276\013\011\320C\031\227\252\220\026\317F\374_\021\011P\236\355\034\325\033\326\272\260{\000\241\2402\0034\330YSw&\331\225\363R\020\b\351\226\000\200\217x\353\370\245\344\266\306\357u\212\273\206I\333\014K\202\\\020\200i\314\336\a\316\260\025\302\377\002\353\345\001D\243\277r\025\272\337P\b\362\b\272;\254qQq\215\251\361\222'\216\030_xzF\000\245\n4\250\307\366\374Tl\200N\303\a\3242l\203I\330\207+\n\316,@E\354/c/B\001\000n\301\244\335-\351\036$\234i\014\213\203\000\201M\201\011:n\2310\000\257\332\264\013;\331M\274\366\263P,\034{\371.\000\200\220\231B\0309\216\272\027m\333,x\362\221\202s_v\030\240\215P\221\360\364\000\020\375\353 \324\372r\235b\330\377+\307>\340\214x\220&\203\r\274\301\252\340\036\013\256\014Wa\nX\215\372\006r\366\316\226L'\274\177\032K\227\240?\244f\000N[&\014\300\027\223\224\354\313{\277\301\016\030\247\003\202\005\370\246@\325\240p\000\\G\223\250\220\225\362\220Rx|\\8\254u8\213\342:\016\000p\277\264\324\361\322F\360\360\267Uh\364\276\210\212\235\"\205VK\026E`\330E\215\027\205\227\003:v\216\300\n\246?\250\267\2475\325s$\355vC\335C\030\001\233\023\242\216\260\372\261q\230\220\017(\rw K\277\324B\376\337\306\370\347.(\034\321M\302G\316\367\001\374\355\034\246B\360\345h\273\026\0326!\251\020\226\005\346\345n\030z\006\212A\362\341\344$\b\317{\201\025\250\033\261\024HXjdl\263\023\323=\310\302cK\034U\345sv\206\360\254\231d\301\375\365\301\301&H;\000\000\0061\036\3427\301\002\030\345\351\3139\001\360U\233\016\374P\365\237z\217.\337\312\202\260\300\330!\240e\bG\346\013\375\027\315\300\205\306y\323B8J\b.\354\235/P\360\335\270\b\314\360\372\256\300\326\331\336y\257\221)/AX\361-\200\205\347o\002\250\022;?\030\217\006G\364\334\241\276\256\227:\n-\030\266GVi@u(\0010\330\363\237Q\031\326\303\031\365=U'\347i\233\036\352\362\374A\216\002=\026\n\332\365 L\004\307\b\264\352\242\022*\207\252o\005/\305`\206\333\2315\"\370\341`\207\035\335I;\a\000,0\004\347\335\341\222\360\342\232\307\302.TgW\322\374\361k\275o\302\330;\020\376u\a\361\313\032X$3\340\214\264\317\002\235+\000\364\224C\273\267[\336c\206$y\360\177\002\000\026\214)\215\217\375\202\326\034\2711\275%\0042\245s\016j\024\264\361\332'\316!<k\234\035\234\2408\013\037\002\325Q\3250\237\343\327%hc \361\213*\262\023\031\372\331\253=G\366'\254f\020\257S\325\250O\266\011_`\204\366\321tf\345\234\001\300k2O\233\364\313\027m\177\027Gd\034\2662\013\330\231\261\300E\301\213\032\227\207\351\315G\241i\026\232\273\342\017S_\000\000\312\313%\341\361\371\253(xQx\316\r<\376\254\236H\3233\373{\333\036=\234z\023\222va\305\355\013z\324\a3\304\256\354\260\355\235\231\374\023\013\204F\217=\213\350\215\237d\334\307*t\251bUHY\310\356\227\277\360\262\321\363\2449\213\023\3478\224\322XNpx\317\237}\003\263\200\253\304\353;k]P\237\217,8\354\035\300\372\370\352\344f\rR\322yz\2459\335\367\355\327\a_\307\016\\\033\336\325U\251S_Y\201\372\216\027\023\270\321\323\033\367\232I{\316\245\035\221\341Lx\336\347M/0' O\232\036T\252J\376\240\230\260\274\305\000\016\\D2\003\206@|\352\302g'\r\332\257\3046\267\2165_\217a\343\223\001\020t\a\2138\226\301/J\274\024>\265\201\366\257\237H\367\337\267/\271\253\0231\b\354\275\035_\316:\260\347\332\2548\324\327}\226\332g\301\317\013\000<P\037\226\240\037\372\244>[\360\354i\232\034\252\017)\223\025hY898E\001\204\020\036\000@\323\314\004\017\301A\027\276`\004\203\370\340\030UI/\017\222\026\301\032\317\375\261a\340Cp\302/Cd\374\216\300G\210\363|s\266\343\233\r\331\327\332-\277\031\341^'0\353\252\325\251\331/P\307\376\011h\377\274\002p\037\226\362\315\240c\207OeO\345\\\336%\310\327\a\345:\254\006:\177\270\300bW\244{\311Q\302,\360\323X\244\260\bt I5\000`\347\3071\263g \236\203\243\223\001\200\016\352'\322\226\365\213c\331\303\337k,\354\351\307\347\006D:]\360\227=U\001:\241\346\251\345M\370!\026f\"\345\2741\200_\276\a\311\337\365\320L\237BU;\363^\376h\326k\217\343\233E\231\"A\267H\022\021\370\343\367\313\342\213\2168\002\200\226\244M1\006\200cg\204\2652\";\035\033;A\234\027,\317}\271\313\350\371\356\301\314\236'\273\355\303\210(\273\260A\323\213\030\253\a\232?!\025\250\351p\321\361\235\261\327\037\r\322y\005\200\a\307N\252\2611Nm\003&\2257\271\276\372J\326mn\261\274^\374\000Z\002\323\261\205\000\223\a\027x\005D\204K\355)\233j\220\370L\301\246c\020\313\035\214\322\3537\375\374\213\335f\367\217\033s\r\2176\033\a[\r\374:\205\177E\0033Ct\334=Y\247&\265@\215G\213k\376i\023\236\321B\217\274f\377\373\216\224\215D\341C*\255\355\301\267Q,Oa0 :E\243\232\271!\245vNH\256\232\024\220\3431M\326\223i[\rEUO\r\310\346\200'\025\032SV\372h\312I\365[~\ni\005~\226@I\b\236\301DSQ|\357\257\nRS,G-`\033\307\374\023\326|I\350w\014\000~\001f'_\241\323\202\026\227\256DJ<\005J\307\202\206\315\035\354{\014W\025_\272\024X\002\177D-}7\361\220\025\362\357\247\013\210g\371Cp\016}\022 I\027@l\216\031\324\003\341\3178\324-\011:\326\361\035\005\240\364R8\307\350\033\032-Jz\264\020_\320\252\261vk\314[\200\301E\314\001\347B\233h\347\243\303;;p\r)\374\342\256/\246Q\a6_{\217\025\277\375s\230q\336\312\273\002@i\266_\234F\241\027\372iZ\302\245\031ph5\0206\006fhH\367y\367\314\301\252\311\277\351)`-H\303_\014Fe$7&\r\202-9v\260\245q\316\347\361]\005\2404q\250X\302\266\232\326_N\341\004\234~-6,\207\260\362\365\025\310\251\216\222\235\311\n \214\206\242\320\202\031\245g/\034/ p\001\201\013\b\234O\004\376\037\363\276R?mt\355m\000\000\000\000IEND\256B`\202", 6440);
		m_BmpCrash = new wxBitmap(wxImage(sm));
	}
}



void lsCrashReport_DLG::_CreateControls() {
 	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer43;
	bSizer43 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer51;
	bSizer51 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBitmap* m_bitmap2;
 	m_bitmap2 = new wxStaticBitmap( this, wxID_ANY, *m_BmpCrash, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer51->Add( m_bitmap2, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_InfoTextCtrl = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	m_InfoTextCtrl->Wrap( -1 );
	bSizer51->Add( m_InfoTextCtrl, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer43->Add( bSizer51, 0, wxEXPAND|wxALL, 5 );
	
	wxBoxSizer* bSizer45;
	bSizer45 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticText* m_staticText31;
	m_staticText31 = new wxStaticText( this, wxID_ANY, _("Please send us this error report to help fix the problem and improve this software"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	bSizer45->Add( m_staticText31, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("description") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer4->AddGrowableCol( 1 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* m_staticText33;
	m_staticText33 = new wxStaticText( this, wxID_ANY, _("email (optional):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText33->Wrap( -1 );
	fgSizer4->Add( m_staticText33, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_CrashEmailCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( m_CrashEmailCtrl, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText34;
	m_staticText34 = new wxStaticText( this, wxID_ANY, _("Problem description:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34->Wrap( -1 );
	fgSizer4->Add( m_staticText34, 0, wxALL, 5 );
	
	m_CrashDescCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,100 ), wxTE_MULTILINE );
	fgSizer4->Add( m_CrashDescCtrl, 1, wxALL|wxEXPAND, 5 );
	
	sbSizer5->Add( fgSizer4, 1, wxEXPAND, 5 );
	
	bSizer45->Add( sbSizer5, 1, wxEXPAND|wxALL, 5 );
	
	wxBoxSizer* bSizer54;
	bSizer54 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText35;
	m_staticText35 = new wxStaticText( this, wxID_ANY, _("To see what this report contains"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	bSizer54->Add( m_staticText35, 0, wxALL, 5 );
	
	m_DisplayInfoLinkCtrl = new wxHyperlinkCtrl( this, wxID_ANY, _("click here"), wxT("http://www.wxformbuilder.org"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	bSizer54->Add( m_DisplayInfoLinkCtrl, 0, wxALL, 5 );
	
	bSizer45->Add( bSizer54, 0, wxEXPAND|wxALL, 5 );
	
	wxBoxSizer* bSizer55;
	bSizer55 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer55->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxButton* m_button37;
	m_button37 = new wxButton( this, wxID_OK, _("&Send"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button37->SetDefault();
	bSizer55->Add( m_button37, 0, wxALL, 5 );
	
	wxButton* m_button38;
	m_button38 = new wxButton( this, wxID_CANCEL, _("E&xit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer55->Add( m_button38, 0, wxALL, 5 );
	
	bSizer45->Add( bSizer55, 0, wxALIGN_RIGHT|wxEXPAND|wxALL, 5 );
	
	bSizer43->Add( bSizer45, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer43 );
	//this->Layout();
	//bSizer43->Fit( this );
	
	// Connect Events
	m_DisplayInfoLinkCtrl->Connect( wxEVT_COMMAND_HYPERLINK, wxHyperlinkEventHandler( lsCrashReport_DLG::OnLinkClicked ), NULL, this );
}



lsCrashReport_DLG::lsCrashReport_DLG(wxWindow * parent, const wxString & softname, const wxString & tempdirectory, wxWindowID id, const wxString & title, const wxPoint & pos, const wxSize & size, long style):
wxDialog(parent, id, title, pos, size, style){
    m_BmpCrash = NULL;
    _CreateBitmaps();
    _CreateControls();
    m_InfoTextCtrl->SetLabel(wxString::Format(_("%s has encountered an unexpected problem and will close. "),softname));
    m_InfoTextCtrl->SetFont(m_InfoTextCtrl->GetFont().Bold());
    m_CrashDirectory = tempdirectory;
    this->Layout();
    GetSizer()->Fit(this);
}



lsCrashReport_DLG::~lsCrashReport_DLG(){
    wxDELETE(m_BmpCrash);
}


wxString lsCrashReport_DLG::GetEmail() {
    return m_CrashEmailCtrl->GetValue();
}



wxString lsCrashReport_DLG::GetDescription() {
    return m_CrashDescCtrl->GetValue();
}




/*************************************************************************************//**
Crash report class
*****************************************************************************************/
wxString lsCrashReport::_CreateStyleSheet() {
    wxString myStyleSheet = _T("body { \
                               font-family: Arial, Helvetica, sans-serif; \
                               background-color: #FFF; \
                               width: 90%; \
                               padding-right: 5%; \
                               padding-left: 5%; \
                               } \
                               \
                               h1 { \
                               font-size: 24px; \
                               border-width: 2 px; \
                               border-color: #F00; \
                               border-style: solid; \
                               margin:0px; \
                               padding: 2px; \
                               } \
                               \
                               h2 { \
                               font-size: 16px; \
                               line-height: normal; \
                               margin-top: 0px; \
                               margin-bottom: 0px; \
                               padding-top: 20px; \
                               padding-bottom: 5px; \
                               } \
                               \
                               p { \
                               padding: 0px; \
                               margin: 0px; \
                               } \
                               \
                               .crash { \
                               font-size: 12px; \
                               font-style: italic; \
                               padding-top: 5px; \
                               }");
    return myStyleSheet;
}


wxString lsCrashReport::_CreateGeneralInfo() {
    wxString myGeneralInfo = _T("<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \
                                 \"http://www.w3.org/TR/html4/strict.dtd\"> \
                                 <html lang=\"en\"> \
                                 <head> \
                                 <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"> \
                                 <title>Bug Report - General information</title> \
                                 <link href=\"crash.css\" rel=\"stylesheet\" type=\"text/css\">  \
                                 </head> \
                                 <body> \
                                 <h1>General information</h1> \
                                 <p class=\"crash\">Crash report generated on : %s by '%s'</p>  \
                                 <h2>OS Description</h2> \
                                 <p>%s</p> \
                                 <h2>Availlable Free Memory</h2> \
                                 <p>%ld</p> \
                                 <h2>Host Name</h2> \
                                 <p>%s</p> \
                                 <h2>User ID</h2> \
                                 <p>%s</p> \
                                 <h2>User Name</h2> \
                                 <p>%s</p> \
                                 <h2>Software version</h2> \
                                 <p>%s</p> \
                                 <h2>Components version</h2> \
                                 <p>%s</p> \
                                 </body> \
                                 </html>");
    
    wxString mySoftVersion = wxEmptyString;
    wxString mySoftComponent = wxEmptyString;

#ifdef USE_VERSION
    lsVersion myVersion;
    mySoftVersion << myVersion.GetSoftSVN();
    mySoftComponent << myVersion.GetAllModules();
    mySoftComponent.Replace(_T("\n"), _T("<br>"));
    
#endif

    
    return wxString::Format(myGeneralInfo,
                            wxDateTime::Now().FormatISOCombined('-'),
                            m_SoftName,
                            wxGetOsDescription(),
                            wxGetFreeMemory(),
                            wxGetHostName(),
                            wxGetUserId(),
                            wxGetUserName(),
                            mySoftVersion,
                            mySoftComponent);
}


wxString lsCrashReport::_CreateAddInfo (const wxString & email, const wxString & description){
    wxString myAddInfo = _T("<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \
                            \"http://www.w3.org/TR/html4/strict.dtd\"> \
                            <html lang=\"en\"> \
                            <head> \
                            <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"> \
                            <title>Bug Report - Added information</title> \
                            <link href=\"crash.css\" rel=\"stylesheet\" type=\"text/css\">  \
                            </head> \
                            <body> \
                            <h1>Added information</h1> \
                            <p class=\"crash\">Crash report generated on : %s by '%s'</p>  \
                            <h2>Email</h2> \
                            <p><a href=\"mailto:%s\">%s</a></p> \
                            <h2>Description</h2> \
                            <p>%s</p> \
                            </body> \
                            </html>");
    wxString myDesc = description;
    myDesc.Replace(_T("\n"), _T("<br>"));
    
    return wxString::Format(myAddInfo,
                            wxDateTime::Now().FormatISOCombined('-'),
                            m_SoftName,
                            email,
                            email,
                            myDesc);
}


lsCrashReport::lsCrashReport(const wxString & softname) {
    m_SoftName = softname;
    m_ReportZipName = wxEmptyString;
    m_Silent = false;
    m_Report = NULL;
}



lsCrashReport::~lsCrashReport() {
    wxDELETE(m_Report);
}


void lsCrashReport::AddFileToReport(const wxString & filename) {
    m_AddedFileNames.Add(filename);
}



bool lsCrashReport::PrepareReport(wxDebugReport::Context ctx, bool silent) {
    m_Silent = silent;
    m_ReportZipName = wxEmptyString;
    
    //preparing report
    m_Report = new wxDebugReportCompress();
    m_Report->AddAll(ctx);
    m_Report->AddText(_T("crash.css"), _CreateStyleSheet(), _T("stylesheet"));
    m_Report->AddText(_T("general.html"), _CreateGeneralInfo(), _("general information"));
    
    // add user specified files
    for (unsigned int i = 0; i< m_AddedFileNames.GetCount(); i++) {
        m_Report->AddFile(m_AddedFileNames[i], wxString::Format(_("user added file : %d"), i));
    }
    
    if (silent == true) {
        m_Report->Process();
        m_ReportZipName = m_Report->GetCompressedFileName();
        m_Report->Reset();
        
    }
    
    lsCrashReport_DLG myDlg(NULL, m_SoftName, m_Report->GetDirectory());
    if (myDlg.ShowModal() != wxID_OK) {
        return false;
    }
    
    m_Report->AddText(_T("added.html"), _CreateAddInfo(myDlg.GetEmail(), myDlg.GetDescription()), _("added information"));
    m_Report->Process();
    m_ReportZipName = m_Report->GetCompressedFileName();
    m_Report->Reset();
    return true;
}



bool lsCrashReport::SendReportWeb(const wxString & serverurl, const wxString & proxy) {
    CURLcode myCurlError = curl_global_init(CURL_GLOBAL_ALL);
    CURL * easyhandle = curl_easy_init();
    if (easyhandle == NULL) {
        return false;
    }
    
    wxFileName myTempZipFileName (m_Report->GetCompressedFileName());
    curl_easy_setopt(easyhandle, CURLOPT_URL, (const char*) serverurl.mb_str(wxConvUTF8));
    
    struct curl_httppost *formpost=NULL;
    struct curl_httppost *lastptr=NULL;
    struct curl_slist *headerlist=NULL;
    
    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, "filename",
                 CURLFORM_COPYCONTENTS, (const char *) myTempZipFileName.GetFullName().mb_str(wxConvUTF8), // "postit2.c",
                 CURLFORM_END);
    
    curl_formadd(&formpost,
                 &lastptr,
                 CURLFORM_COPYNAME, "filecontents",
                 CURLFORM_FILE, (const char *) myTempZipFileName.GetFullPath().mb_str(wxConvUTF8), // "postit2.c",
                 CURLFORM_END);
    
    curl_formadd(&formpost, &lastptr,
                 CURLFORM_COPYNAME, "softname",
                 CURLFORM_COPYCONTENTS, (const char *) m_SoftName.mb_str(wxConvUTF8),
                 CURLFORM_END);
    
    curl_easy_setopt(easyhandle, CURLOPT_HTTPPOST, formpost);
    
    if (proxy.IsEmpty() == false) {
        curl_easy_setopt(easyhandle, CURLOPT_PROXY, (const char*)proxy.mb_str(wxConvUTF8));
    }
    
    // read response
    wxStringOutputStream  myBuffer;
    myCurlError = curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, wxcurl_string_write);
    myCurlError = curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA,(void*)& myBuffer);
    
    myCurlError = curl_easy_perform(easyhandle); /* post away! */
    if (myCurlError != CURLE_OK) {
        wxString myErr(curl_easy_strerror(myCurlError));
        wxLogError(myErr);
        
        curl_easy_cleanup(easyhandle);
        curl_formfree(formpost);
        curl_slist_free_all (headerlist);
        return false;
    }
    wxString myPageInfo = myBuffer.GetString();
    wxLogWarning( myPageInfo );
    curl_easy_cleanup(easyhandle);
    curl_formfree(formpost);
    curl_slist_free_all (headerlist);
    return true;
}



bool lsCrashReport::SaveReportFile(const wxString & directory) {
    wxFileName myExistingFile (m_ReportZipName);
    wxFileName myCopiedFile (myExistingFile);
    myCopiedFile.SetPath(directory);
    return wxCopyFile(myExistingFile.GetFullPath(), myCopiedFile.GetFullPath());
}



size_t wxcurl_string_write(void* ptr, size_t size, size_t nmemb, void* stream){
	size_t iRealSize = size * nmemb;
	wxOutputStream* pBuf = (wxOutputStream*)stream;
	if(pBuf){
		pBuf->Write(ptr, iRealSize);
		return pBuf->LastWrite();
	}
	return 0;
}

