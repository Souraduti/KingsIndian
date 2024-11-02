#include "zobrist.h"

/**
 * 12 array of 64 bit integers of size 64 is used for zobrist hashing 
 */


long long int w_king[64]   = {0x7f17c1c7656a459f, 0x44a3c0c0e7f026ad, 0x47f840c46f69363f, 0x8dfe66cf71dd50cf, 0x449b87cbea79125f, 0xcfb1ca15a0753d51, 0x43024cdb176dc661, 0x238a6708289028c3, 0xef091a623e4656e6, 0x520b41614ea7b052, 0x61e27fd5113a86b2, 0xf85535f8115354f7, 0x342ba781c14ae933, 0xbfb47ab53e52dd9a, 0x3cf84a52c279115d, 0xc9505d7383cb8e69, 0x9da81cbcef7976fd, 0x278269b1fcd51280, 0xd33b2a46cdd5323a, 0x65b2cb7d93b8f65, 0x766d541fe7f631d0, 0xba4110f53bf89593, 0xd5415ee558b97bf5, 0x2a336d4b451a694e, 0x52851497f3efdedc, 0x6f87bebf7bcdcd8a, 0x5159755044ffdc7d, 0x1fc87aedc7b464a6, 0x4c92794625607586, 0xcb213c0dc0fcb1ac, 0x50ce228f3fde7e02, 0x5b525e0359f23e9e, 0x230b09a49020a963, 0x991efcf9371d6d09, 0xd4bc5469124da0b5, 0x443027c073c2a79d, 0x6789bbf3df425a38, 0x8459481aace32a9f, 0x554751ddca92a93b, 0x8bd0acce098786c4, 0xb2a6da40540957fd, 0xad189a19622b5cf6, 0x5f62451ee54cc0a1, 0x7216b91714b9fb0f, 0x7d9298b553a6e88b, 0x1eb8873ff9e51168, 0x4d9f6c319ec8ba8e, 0x5504f35df665561a, 0x6efafb9974964b29, 0x8c2f8eb40fdd74df, 0x5d79dff4db080d8a, 0x995262a369f888d9, 0x4863566d3e2c9207, 0x78b091bbc22e8f37, 0x486aa254c47d8e63, 0x699b1f72382f4539, 0x12dfae9623b2d1cc, 0x785004b15135f996, 0x8d4d5cc541cc413b, 0x9d899aca21902f0, 0xd61e818d13bb24b9, 0x7272d517fd210f50, 0x62a8344495a68e4c, 0x6e344d7d7ebdfbd0};
long long int w_queen[64]  = {0x5a00e5086a37d453, 0x34f2402241b82e2c, 0xca32498612c03a18, 0x487445f42defff65, 0x1ecc5fa4db4ff721, 0xd2535d07d8b7006c, 0x9f7c506f42a8be4f, 0x414d4df6403674aa, 0xbeeae0b12b507dc3, 0x2debc3cc4e6553ab, 0x37abdd81a5918d4, 0xe2e9ec3c1dafa233, 0xb53dc151e117a6b2, 0xade9e717faeda17e, 0x1a4778c2f9d5bb2b, 0x18d30f412a2cbc1f, 0xa5f0bd87ce8dbed6, 0x56b7a2467f3477bd, 0x452c7dfca5a100f6, 0x50e5f057634d18ac, 0x3958fd99ef4b7492, 0x448569873ea4127d, 0x4968b247be7a5018, 0xdc5c56318f8fe83d, 0xf8f2ce185a95a89d, 0x1fbdf16023304a5d, 0xf451b8193df4a598, 0x63d22345d7dc61d4, 0x72395f5755acd14a, 0x4a61c26544c8de40, 0x787b8d75636eae84, 0xc8011b2109a9b41, 0xb96a66ada52691a3, 0x9212312d000cab18, 0x6e4e27386ec0fd90, 0xce3b65f99ef26e30, 0xdd65db15b95d5da3, 0xb247761263697175, 0x10e5c5069ff334e5, 0xa5763681d7a7c9ff, 0x9371283966291081, 0xc78329f2cd6abf70, 0x258a8ef8614dbd4a, 0x2d5a7d4905af63aa, 0x48d2478ac4da87f3, 0x635c25279e59b1c8, 0xb0b29de31cdba9ab, 0x1d2ce6d4880d8143, 0xe82cfaeacef0492c, 0x84f255e0bc5013e6, 0x978049d30db32aa0, 0x2aa74682d533b185, 0xa5aab02ead9e7f82, 0x31fd8ff09e3b152f, 0xd7d59ce8aabd49cd, 0x6bc48ec6f356eb2, 0xd7a746c010698115, 0xb95ffa4eb0f9f67c, 0xff061ac77b097ec9, 0xf4041ee017923fd8, 0xc9ef088d35389503, 0xd756ff4e358ae485, 0x88a63d3738762a41, 0x33fdf0eda505671};
long long int w_rook[64]   = {0x6023ae6e58b30c17, 0xa3691cf8ff59763a, 0xe5b09abea4b3980, 0x82b4edbe06cc466c, 0x33deaa4000261674, 0x74e43dfcebfb8ab3, 0x302c3b25c34cc0f7, 0x64f8b96517710905, 0xc7060d579f050488, 0xb3711c1579f1d4c5, 0x7b75fc65a876c3b7, 0x3d291e54eefe30f9, 0x3bb91b49a1c219be, 0x257c93b9a791f9ea, 0x99f4f79909202cc4, 0xd5931cc912d1ba2d, 0x7b15bb176b24ebb5, 0x7b72bc1635bd61ec, 0xd05f844decf0f1ef, 0x24293f34b390525c, 0x9935db6035a4d772, 0xdfbeed012e5e211a, 0x11b8d6aa916378c4, 0x1179890899e1e5a8, 0x829451c936018b72, 0xf48ffb7318cddebe, 0xf1a3be7d44eb5b54, 0xd23d45bfd10649a, 0xe8e22c7534b7e2ee, 0xf8a25d13d67ad454, 0x3a9382146865adee, 0x144441ce309015a8, 0x230876d457a9f9d7, 0xaf063f02d2c5225b, 0x1abb0882fabde17a, 0x1b9950de9fdfe97d, 0x9275be5ab9f33d85, 0x168cc245d496d561, 0x62aa5a1560fa817, 0x10198753563bcc72, 0x2287cbf539d0b9f3, 0x3e69fc62384c989, 0x5acecf094395697a, 0x170f6b9bd91b3fa8, 0xfd8401d524dffd04, 0x104bc2d49ac2d3de, 0x34b783d35bbe4360, 0x9abe10bae277f209, 0xd72d56ad79179230, 0x52c551ceeacca79a, 0xd4f58436df8ca28c, 0x5f515cc883fc6f81, 0xb548869e0ec1f4a, 0x4e6d08c0a98c0c06, 0xf69465b0bb5e6be3, 0x5262f727f70609fb, 0xd3da029ec60ac9f0, 0xf4727a6e9ae19f0b, 0xce1ea5f8063bf61c, 0xb956c29538f989a7, 0xf9e562b022d1c3, 0x25dda60160cb4fce, 0xd9f9797f2433aeb9, 0xc9558b330427e116};
long long int w_bishop[64] = {0x20947463e65480e3, 0x91b84bf8afbdad33, 0xfad05b4acb86a37c, 0x82445477285af016, 0x6c1912e160bfc0db, 0x5e62167153a1493c, 0x6e3e873115d014a3, 0xac7a88b85f509115, 0xbddb76f43acaac1, 0xdb080dfcc0cf02d, 0x237cf71981cef90a, 0x67098551d4a0a2dc, 0x58732d358d07f6e, 0x68fe2cc6ad43035d, 0xbd0dc40c958de008, 0xda2bcb227cb2b2c9, 0x220089c58174a856, 0x98dad2af3c853b14, 0xfb2acb37aa7400a1, 0xcdefc1c8a05f2e11, 0x38bd3e38271b763f, 0x5ffe7fb6d396cc19, 0x4775777374fea153, 0x52ee2ff2aead37a9, 0xf69c89336c2d7177, 0xfa2e0bbfc22d5c95, 0xb2e05c1e33483095, 0xf0fc379b44e996bf, 0x27b71c8e8c45ef64, 0x792666ee8985f719, 0x11f5e4b6f6611abd, 0xdc8aca901c11f613, 0xe347432d70848c81, 0x56fc5a7d69216904, 0x921381df9e53e1dc, 0xf45779d7c34730ca, 0xedd4356f41e5eb4d, 0xe74acfb0cf1f721f, 0x5f610f2ae030b73b, 0xfaaf239d9dd0960e, 0xc704bf456e0a77bc, 0x8a7efa4914e5f376, 0xb378a86d34ba9d27, 0x1c7eece3c52c8078, 0xecf75009246a5a4c, 0x50ee8782d8b9313d, 0x7932e62cd3ce48ef, 0xda811153b81b6414, 0x6ab653f2c05191f3, 0x27c4a882377e2c96, 0xfceab6e28ca70b2a, 0x87faa895af30aaf0, 0xf506331ddd55f071, 0xeace281af90e53dd, 0x6851da403a518606, 0x7b078a5ad09d3199, 0x2d7d686688dd367e, 0x76e3bf063362d911, 0xfc9c0cc7508e6335, 0x6c991bbc6ddbd860, 0x81c019a8733498f9, 0xe18d9208ff5dc06b, 0xe12286b6d96381e4, 0x891c00a0c036db8b};
long long int w_night[64]  = {0x57456f3f7d72e148, 0x97e5628bec74f737, 0xf3f8a06173b7548a, 0xfac36f00744b935f, 0x92137f117e1bcdff, 0xc388027316c2485e, 0xabddc988b225b455, 0xfadeff7ae53891c3, 0x11c0086fe57479f3, 0x2a5e48642c9ec15a, 0x30887ba29d2afbfc, 0xdb37c99de9b8d8ad, 0x8c7f5b2cbb2051cb, 0xef063af4a6f72b51, 0x5b3634985b9d7860, 0xa94e8f4f74b72bb4, 0x3226a714894abf55, 0x4352fdad4eb7755b, 0xaa929bf6c9f25047, 0xfc03b7c137d1b6b7, 0x7fe5447d40a0fffe, 0x5b709541442be82e, 0x549bc30dff8e7e8c, 0xbf38d04fbea76a20, 0xe0e4ba00ba439501, 0xeb726077a3e94d18, 0x68a03c08d10a70d4, 0x7fac312a0d923323, 0x4baad101bfb970b1, 0x6ef8b661cd3dcc12, 0x191251a07e45cb9f, 0x6596847ae5b30a5f, 0x672e4e5491f85cf6, 0xdb04bb7ea604445c, 0xe25c55f6fc7fdaa, 0x6c8fe96d59d2cc11, 0xb8549fc306fae91a, 0x40866bf07502cfbf, 0x489ec8d85b5fed3a, 0x68da66e15f2556ad, 0xe6d7594d79b6cd0, 0xde6b07baca5a9201, 0xfa3ac2fdad311457, 0x552bf8391be4bb10, 0x64a7af2a89e2757f, 0xb7fc9333694d8066, 0x3405f298fc74154e, 0x5d3f688ee3d9a218, 0xcf291eeecfc5f096, 0x699037732dae8d25, 0x2d3fa7c386f6e289, 0x77060ec1f5280444, 0xd12627d2ec08cf33, 0x95c0bf1035cf3ebd, 0x7647b0aed3e932fa, 0xff9db1cfd334009f, 0xe3e49b353007c5ee, 0xf475d6c7cfc9e025, 0xd90f51fd21e749ff, 0x52b8087f6cab83fd, 0xee30c9a22c968a74, 0x5c90097d710c1825, 0x2ab7df2788fed64b, 0x9269ff7e7c97fd94};
long long int w_pawn[64]   = {0xe532905246e13003, 0xf23a439961ec2923, 0x1a7008e40d13ec6, 0x7eb3726180561c76, 0xb12a7da11cb2d517, 0x434b12beb905b838, 0x833224b03bdc8ce, 0xc80915425696598f, 0xdff5c47c2565dc63, 0xc62c3731467adb3b, 0x8a03138c3495a6cd, 0x78a2355fcde9748, 0x2f56992cc052f6eb, 0x5f183ba92f92fa98, 0xb5dcc8e5636ef277, 0x32877cdda3155c6c, 0x2343b41dd00dc4cc, 0x89a36d99077ef70c, 0xbe6116aa97363aeb, 0x9b77470484edfcd3, 0xa9267a922890e1b7, 0x34eeabb34ae07c29, 0x15529928faa6b462, 0x23df1962c750e1a, 0x2c2dca6ea6d04188, 0x773c2e83d6cc05cb, 0xad65769eeff038e1, 0x149b5356450016e0, 0x4c9387eb583365ed, 0xef008249f5e497cf, 0xbd3f1bb984ff3dd5, 0xff3c01186c80797b, 0x7a2b977fc2eb3259, 0x21a6a09ff11534ef, 0xe34d31f2cd0470b5, 0xb87c6c1cdacc2d64, 0x3c6786141e1e0a76, 0x23593c10920edd24, 0x2baea78b4735a89b, 0x37b3b5cf66c3655a, 0x4743c6d10643411a, 0x2d3955a99e3bfc20, 0x51ec76f5ad359302, 0xaff892a9523edbc4, 0x87869533f3f04259, 0x1942b8e21bb15172, 0xa99f376ece4c36bf, 0x843a8a334cedd4ef, 0x3d2ca694591cf2b9, 0x7aa4458b155e8971, 0xe404d01191ad3d70, 0xe06a3e8b1baad527, 0x3f49d507dce22255, 0x89d653fc6c3e92a3, 0xa31712cf14abdc38, 0x944cdd6fc3be6b9, 0x1d2ba0b48f90a794, 0x1549037e29ffb734, 0x77eb92e65b051aea, 0x79c98b1acd4496e7, 0x6b3c4259e06eb2be, 0x3d14e36979306cb2, 0x42dae2e81a2948ca, 0x60de12686a151549};

long long int b_king[64]   = {0x50e6fe8c6ebb4c83, 0x2320b36c07f105be, 0x60d52c05905b15d9, 0x68710932d4da84a8, 0xf8629d9a9fc5d19c, 0x519612e8f21fc9fc, 0x396bf4e4ae042306, 0x890808ebad38950e, 0x57ef4f8c04ea2f9a, 0xe8eaa42959cb3b0f, 0x20f042caeb524be6, 0x618ec9d4df220163, 0x8bf59e7449004fba, 0xd4c2e4c8dc350c7c, 0xcd0c5b785f105838, 0x89bd347f7515c73a, 0x8a5b2e4b30b8161e, 0x3079aad11b4c7506, 0x9d7de9f6803823b7, 0x4109f2b341212e66, 0x938f1c1e6aa3ab8d, 0xe67f32ba5c8e90ab, 0x123f5cd8b6d867cc, 0x649e3260b5da2eba, 0xf935fbd52192346c, 0xb8553bdfd36223f4, 0xfd4368bc11ddcb78, 0xb9977f4dfb5de2a5, 0x8aad860dfd1c27b2, 0xd1eb95c99ea512ec, 0xe2fbb2ffa27addcf, 0xfe076ea6adc6b7dc, 0x590580ad56599271, 0xcd5916863e713a5b, 0x37ba1bb911d8ca61, 0xbcd07e2a2d7ab07a, 0x8140509ce85e6d1f, 0xcd532cdbffa91313, 0x1343f37fa177c433, 0xb687eb6e89dbd69a, 0x7f3b33c8bcdaef48, 0x405d02982385b2b1, 0x471640c7deb63685, 0x87eb67f771bbe4aa, 0xcfe25db8e738f25d, 0x7023b601257b679e, 0x7b871d0e88d8fbe5, 0x61522062c1f5cdee, 0x160a002f2b62d52e, 0x49c487a46364bec5, 0xf080b076661f8ab4, 0xc9385ee4a24072f2, 0x774643817350551c, 0xb7ca8d9d54056b1d, 0x78daf0dfc5e2f125, 0xeaa9b665fccbddc6, 0xed3a52b239626ec0, 0xaf866f74f0d8b2a0, 0x9e13e2fef11838b5, 0x47f6783e62b6a273, 0x408c58fc0246f64d, 0x589a573f5d01d7ae, 0x145267b038104686, 0x6e0c78d38ea0fdf1};
long long int b_queen[64]  = {0x7263c299128829b3, 0x8bfbbf4b82f5412c, 0xc163098b707a9ee0, 0xceca914251d4ee54, 0xe0264e48fd05ff22, 0x805c14781cd5461c, 0x743bc0dbde1b99d1, 0x4f8ea67ea513c618, 0xf90991373a549e69, 0x464e775c2c71d39d, 0xcff62a682c6daec8, 0x812a036e98dcec26, 0xe44473c1b08f2ea4, 0x998435c772d90552, 0x3eb2224adedb1d14, 0xc7180e98886c775b, 0x3d3c39159ccfeecc, 0xb1f5a320fed88740, 0xac4d9547a944d8a7, 0x67fa493b4cad206c, 0x4d1a22da8776ebca, 0x86313a3ef4b85e7b, 0xb7fe737c8d99f546, 0x78758a7c1fe2455a, 0xf2ce5ebbfbd09c76, 0x43d1dfebb402a15e, 0x2f9d248e71e9e581, 0xfc81f602569dab8a, 0xa4f79187b3605ab5, 0x6dc4b518b7fc36b1, 0xe268fdf547ecceea, 0x9c2be16a0761a87b, 0x272cafa08a15e03, 0xe717c6a59b1a5a0d, 0xaf3ee3771a222bcf, 0x4128611b80f1fb76, 0xce2efc49328ed8d1, 0xc9ad17d8bccd0f19, 0xd5c44b1b358b97e8, 0x751d81e8bdd8fd4f, 0xa09f0a4fcc6323e0, 0x4f02b1609211df3, 0xf4b85f601cfee19d, 0x8756e6a18dc91179, 0x986745ca5b3eaf83, 0x220c520e407141c0, 0x24c33946536711a3, 0x8c7f477c462035ab, 0xd245cf218cac8304, 0xcc23acdd858b6861, 0x7df5025091960ce8, 0x40164682ff037917, 0x5407dfa20cf09a29, 0x191a6126c90b8349, 0x715174a38157a3eb, 0x507ee1e285ed212f, 0xef49326ea8bb52cc, 0x986cbb7c9be233f0, 0x91ac07457ca182c8, 0xc45c616a150847ce, 0x938284a8df886ba8, 0x5fd5fab4ed35e41b, 0x151ad0deee26d222, 0x50abc5aa2fefe48};
long long int b_rook[64]   = {0xf3bcceebb2135c1, 0xe36acab29539f2b9, 0x236ed75eea248386, 0xf596ad03daddd80e, 0x8e0e40178f4d1357, 0xf58463403e62b302, 0x2292f6bd65e08489, 0x6fc34683d1918898, 0x1d8ffc958df8a37c, 0x1a12675be13dda69, 0x2daf2f7dfde90a60, 0xae1b6b6090df1b59, 0x1879ffe46b759ad8, 0x5519884a777e23ae, 0x50c84bf1379114fa, 0x37266344bc3f0f4d, 0xb9eca65dae9d2d1a, 0xeb18c0ad1814813a, 0x20317541ab8bb6a9, 0xc093522877262ca6, 0xe54af976322fbd4f, 0x978c287b637d5dff, 0x2d77a687ced83e60, 0xf75b592d2bd8e8ee, 0x72b245db5d0453a6, 0xa5a1edc6f5144f6, 0xc0dcd624279e8989, 0x14fa1ba3e6b26b23, 0x5473a4aecc21c3fd, 0xc007c960d1b33f9c, 0xf65fdc5f932c9890, 0xe268872ec81657a8, 0x5f9520e00435b599, 0xc9ee050fb36db2a6, 0x4a10707fcee37bda, 0xd6117288ae6c5f57, 0xe068024bff5483aa, 0x94d74d5764faf617, 0x5128668ceaee55ed, 0x2341020cf6468217, 0xf91ca2600c37a3bb, 0x91e4a669854ead0a, 0xf3e3e8a12e1ea62e, 0x8265a3919180267d, 0x6147aadcaa68f37, 0xb3d6c564400678bd, 0x26a7caf5f5ddb2e3, 0xbdadd5932ce0b440, 0xa066daff98a0b9a7, 0xd216053185824bd8, 0x851a725c5254f8a4, 0x6a7a0a4d5b2292be, 0xccc5fed5bb7661d8, 0x69eaa7cc06681524, 0x7a21683511bb38b2, 0x1405506bf465b172, 0x691dce1ad7bfaa02, 0x105385dd02b2e456, 0x99442b5e0983a077, 0x7678e630ddd71323, 0xf05399f27ac2209c, 0x17c145df93cfe25c, 0xb08ad7cba0eddf72, 0x64d9809d9e9afcc6};
long long int b_bishop[64] = {0x7e083cf7775cca79, 0x858c2c5d33c7b422, 0x53959ec1964fd42f, 0x885cfb40d82bcdce, 0xcf9e9e504f73fb8a, 0x956fae6a5983d6df, 0x9e548e48d4acec5e, 0x9247d8c8abf5b1b9, 0x82500cbe03e54b2d, 0x83c339e0c76b5ffb, 0x52e5abe65ecdd9c5, 0xe207ccf7d8d300d6, 0x72f6e72d08a31acd, 0xb5a4f39f67cdb7d5, 0x8af9d8eda1dd6cd7, 0x81ffbcd822688ba, 0x33434a49bd60ccad, 0x66f57a6ff82aef7c, 0x1a5e883e347c2016, 0x65959904e73bc843, 0xbfb9d8c6961b57f8, 0x954074f4d4997d5f, 0x314ae6ee5e99195a, 0xde5f37ad80ae304e, 0xdbca657517474728, 0xb77a49009657045f, 0x7d64c84c624eda1e, 0xd6fcab3f49cf5d56, 0x7c9f719ef93c45da, 0xb89d68934b60d4fd, 0xc10a2db8c1964143, 0x14faad50abbc805e, 0x33ea3dd2435531ef, 0x5557b1b43ebeb6e6, 0x726a47f8955030f8, 0xed7374ebbac91d17, 0x77a67f27606cf5b3, 0xe1054b85598cb205, 0xde035a35a9a877e3, 0x4c2b83bf01516278, 0xcddc28ba0be1844e, 0x19db5485224df669, 0xf0e2bd098467f5fe, 0xb7aab0ac4dd37d57, 0x2c1615f4939e21aa, 0x2f16a12e6bc9d4ac, 0x1ee30dbbdbaec1f7, 0xc5e3e08704f1f442, 0x6369990ff2e9d586, 0x41d8afe377ced9c5, 0xda33ecc62f8736e8, 0x234db98490fbd51, 0xe94c62fd13c8aff8, 0xab12dfce87021d58, 0xf410bff3392c11d1, 0xc9bd73ed807b1a41, 0x516380fd29190d7e, 0x29f42f6406b563e8, 0x9679db2cbf6f7ceb, 0x4f53024a9ec27393, 0xdecb923ca8a027b1, 0xf8f62366a626a4d9, 0xfa8172f5971d7b6b, 0x7570d8f13eb53ca7};
long long int b_night[64]  = {0x3dc0653afcd3ab2d, 0xe994ff6136c557b, 0xc3cd9bba7630fb7c, 0xcb1b094f8789dcda, 0xace81c876b44572a, 0xd35a200702e0d6d5, 0x608420fa5c5abfb0, 0x7ecfe8873a556c69, 0x228fecd220ab4220, 0x52c59b3d6c4bcdf, 0x88c3f58dcfc21e5f, 0x6cb207b358b3cc92, 0xa61f4da9401d7dde, 0xdcf456ba807dc293, 0x5627659085a2918c, 0xf2d0d407e3ff958c, 0x8c5ba653fdf7c86f, 0xc9eb86a2775cd5f1, 0xd8d06c9b3fa683, 0x9d4207b4b39095b8, 0x4642b842e104b680, 0xe656e0cbd1957c86, 0xc4edcd083b9976e, 0xbb6911572d2d357e, 0x3de42cd723456bf6, 0xd9be253c823f8531, 0x3d0f26468e9aa077, 0x8647f5e322f4ac6b, 0x21202a1013b02c07, 0x88cbc0750df6a3ac, 0x73ccfd0fa5f5343d, 0x1d123c44e7a67175, 0xa71c26961ad59fb7, 0xc1360b6252f9f69f, 0x949703a0c0ace94, 0x53776900ed9ad3dc, 0xb29ff684939a92a5, 0xd009c30b9980d46, 0xd6c6e46b4548943a, 0x5d1395106f67b902, 0xc5ef9711679afcd4, 0x2a29c972dfb60671, 0xfc3f48af9e55741d, 0xe2fee4e3ddf7c258, 0x9e6bf67b76bc4bcc, 0x9a532f14508df909, 0x39fe4d1327386fe2, 0x729aae5bb35dac08, 0xef0992b3c5ebf78e, 0x100a00c755abcad, 0x59846f1ab8df64f4, 0x3c30888420648bf6, 0xa2fb9510777a6ed6, 0x99410c22d39920b7, 0xa3eaa2c546487f05, 0xd5ba0337eab8eb69, 0x2eefc551bac4e63a, 0x4ad2ce1897a94a91, 0xd0be6395b8252247, 0x86db72d8860caf9d, 0xd50639411e65f1e9, 0xefaf5e3d8c1bdd00, 0xb55de5785795034e, 0x4eec27f4d35e8b36};
long long int b_pawn[64]   = {0x148dab9ad503720a, 0xe2c3ae3440cb9b19, 0x5c8a558733e74a30, 0xe5bc17550a64484, 0xfe9a40d2b478b983, 0xab373ab8c279df6b, 0x11bc2cef1692db08, 0x317494db7cc066a2, 0xb95cf4fa3bbd193b, 0xa2a9ae446ead88, 0x207f077ad5eabc9c, 0x401372105b939586, 0x531a75aafc78cea, 0xef878249ab4fdd81, 0x280e482bac0109d7, 0x3a3cb84ca103663f, 0x614aa239e2a06aa3, 0x4d132e2ab4e2cee6, 0xd64f47a7bd0c6b36, 0x4b6e592b14f270c4, 0x896ef38491d52292, 0xac1d2609a7f66b1a, 0xd3fdd7bfd550528a, 0xcaecd1cb50075e8b, 0x945a37e5cabb44f3, 0x469f6de12243f8ff, 0xf8f52f091c79edab, 0xb5be8be6763918f, 0x2e4f61b937e85ebb, 0xdeb606fbe6f94011, 0x4deebcd3c84576d, 0xf8ad3303d02a7cc3, 0x30a953c09b3baaff, 0x6b8af3fd373ae41a, 0xfb329f6401ac3db8, 0xe980af27c58e27c, 0x47d2d8bdb29f02f6, 0xb287d19a2e668b00, 0x6cf2881fc474d58c, 0x1fa681788ebceace, 0xedcde3db3acd85d2, 0xd17b8d46d8db2471, 0x857d5677c50a0a8e, 0x9ac771452144cdcb, 0x8072f28185475938, 0x17403eb4b234e637, 0x81f4788aed7e24ea, 0x8ece6299aebb2e05, 0x10004ee06106fae6, 0xfd11bbcc8f3df7b8, 0x1e338d2c1d982f17, 0xb7e014b7d46b7ab5, 0x279ea179e845901d, 0xd8dfc7b1f86811f5, 0x322645b35148652b, 0x599e9b6660883af0, 0x3808d46c1abd7201, 0x7a8c5d9ee3d014ac, 0x134b322764d52cb5, 0x489b1b5151c71de5, 0x10b7243a3ecd30db, 0x34d11f5ec902388d, 0x6eef948981be5e66, 0xdad4a3de0994d886};

long long int get_hash_value(Pices piece,int i){
    long long int v = 0; 
    switch(piece){
            case King   : v = w_king[i];break; 
            case Queen  : v = w_queen[i];break; 
            case Rook   : v = w_rook[i];break; 
            case Bishop : v = w_bishop[i];break; 
            case Night  : v = w_night[i];break; 
            case Pawn   : v = w_pawn[i];break; 

            case -King   : v = b_king[i];break; 
            case -Queen  : v = b_queen[i];break; 
            case -Rook   : v = b_rook[i];break; 
            case -Bishop : v = b_bishop[i];break; 
            case -Night  : v = b_night[i];break; 
            case -Pawn   : v = b_pawn[i];break; 
        }
    return v;
}

long long int compute_hash(const Board * board){
    long long int h = 0;
    int i;
    for(i = 0;i<64;i++){ 
        h = h^get_hash_value(board->brd[i],i);
    }
    return h;
}

//works for both making and unmaking moves
void update_hash(Board * board,Move * move,int unmake){
    long long int new_hash = unmake==0?board->hash[board->move_number-1]:board->hash[board->move_number+1];
    if(is_castling(move,Long)||is_castling(move,Short)||is_promotion(move)||is_enpassant(move)){
        new_hash = compute_hash(board);
    }else{
        new_hash ^= get_hash_value(get_piece(move),get_source(move));
        new_hash ^= get_hash_value(get_piece(move),get_destination(move));
        new_hash ^= get_hash_value(get_captured_piece(move),get_destination(move));
    }
    board->hash[board->move_number] = new_hash; 
}

