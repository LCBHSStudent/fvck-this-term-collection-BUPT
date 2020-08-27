/*
 Navicat Premium Data Transfer

 Source Server         : localDB
 Source Server Type    : MySQL
 Source Server Version : 80020
 Source Host           : localhost:3306
 Source Schema         : pokemon

 Target Server Type    : MySQL
 Target Server Version : 80020
 File Encoding         : 65001

 Date: 27/08/2020 14:27:22
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for pokemon_info
-- ----------------------------
DROP TABLE IF EXISTS `pokemon_info`;
CREATE TABLE `pokemon_info`  (
  `PKM_ID` int(0) NOT NULL,
  `PKM_NAME` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `PKM_TYPE` int(0) NOT NULL DEFAULT 0,
  `PKM_ATTR` int(0) NOT NULL DEFAULT 0,
  `ATK_INC` int(0) NOT NULL DEFAULT 0,
  `DEF_INC` int(0) NOT NULL DEFAULT 0,
  `HP_INC` int(0) NOT NULL DEFAULT 0,
  `SPD_INC` int(0) NOT NULL DEFAULT 0,
  `PKM_SKILL_1` varchar(64) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `PKM_SKILL_2` varchar(64) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `PKM_SKILL_3` varchar(64) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `PKM_SKILL_4` varchar(64) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `DESCRIPTION` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`PKM_ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of pokemon_info
-- ----------------------------
INSERT INTO `pokemon_info` VALUES (160, '大力鳄', 0, 1, 3, 1, 4, 2, 'DragonPulse', 'IcyWind', 'Outrage', 'Liquidation', '十分耐寒，不畏冰海');
INSERT INTO `pokemon_info` VALUES (195, '沼王', 1, 1, 2, 2, 5, 3, 'MegaPunch', 'SurgingStrikes', 'AppleAcid', 'MaxSteelspike', '就算脑袋撞上船底或河里的岩石，也完全不会去在意');
INSERT INTO `pokemon_info` VALUES (254, '蜥蜴王', 2, 2, 1, 3, 4, 2, 'MegaDrain', 'Agility', 'QuickGuard', 'LeafStorm', '拥有改写现实的异能——repaint');
INSERT INTO `pokemon_info` VALUES (726, '炎热喵', 0, 0, 3, 1, 3, 3, 'LeechLife', 'BodySlam', 'SwordsDance', 'FlareBlitz', '如果敌人实力强劲，它便会更有战斗的欲望');
INSERT INTO `pokemon_info` VALUES (812, '轰擂金刚猩', 1, 2, 3, 2, 5, 2, 'GrassyTerrain', 'SolarBlade', 'Reflect', 'BulletSeed', '能够通过打鼓来控制特别的树桩中的力量，操纵树根进行战斗');
INSERT INTO `pokemon_info` VALUES (815, '闪焰王牌', 3, 0, 2, 2, 3, 4, 'QuickAttack', 'FlameCharge', 'Counter', 'FireBlast', '会用猛烈的火焰来燃尽对手');

-- ----------------------------
-- Table structure for skill_list
-- ----------------------------
DROP TABLE IF EXISTS `skill_list`;
CREATE TABLE `skill_list`  (
  `NAME` varchar(64) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `ALIAS` varchar(64) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `DESCRIPTION` varchar(256) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  PRIMARY KEY (`NAME`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of skill_list
-- ----------------------------
INSERT INTO `skill_list` VALUES ('Agility', '高速移动', '让身体放松变得轻盈，大幅提高自己的速度');
INSERT INTO `skill_list` VALUES ('AppleAcid', '苹果酸', '使用从酸苹果种提取出的酸性液体进行攻击，80%降低对手的防御两回合');
INSERT INTO `skill_list` VALUES ('BodySlam', '泰山压顶', '用整个身体压住对手进行攻击。有时会让对手陷入麻痹状态');
INSERT INTO `skill_list` VALUES ('BulletSeed', '种子机关枪', '向对手猛烈地发动种子进行攻击');
INSERT INTO `skill_list` VALUES ('Counter', '绝地反击', '当前血量越低，攻击伤害越高');
INSERT INTO `skill_list` VALUES ('DoubleEdge', '舍身撞击', '拼命地猛撞向对手进行攻击。自己也会受到不小的伤害');
INSERT INTO `skill_list` VALUES ('DragonPulse', '龙之波动', '从大大的口中掀起冲击波攻击对手');
INSERT INTO `skill_list` VALUES ('FireBlast', '大字爆炎', '用大字形状的火焰烧尽对手，有几率让对手陷入灼伤状态');
INSERT INTO `skill_list` VALUES ('FlameCharge', '蓄能焰袭', '让火焰覆盖全身，攻击对手，积蓄力量并提高自己的速度');
INSERT INTO `skill_list` VALUES ('FlareBlitz', '闪焰冲锋', '让火焰覆盖全身猛撞向对手，自己也会受到不小的伤害');
INSERT INTO `skill_list` VALUES ('GrassyTerrain', '青草场地', '五回合内变为青草场地，自身获得小回复，草属性招数威力提升');
INSERT INTO `skill_list` VALUES ('IcyWind', '冰冻之风', '将结冰的冷气吹向对手进行攻击，会降低对手的速度');
INSERT INTO `skill_list` VALUES ('KarateChop', '空手劈', '虽然忘记之后就再也想不起来了，但还是建议忘记这个招式');
INSERT INTO `skill_list` VALUES ('LeafStorm', '飞叶风暴', '用叶片向对手卷起风暴，自己的攻击力降低一回合');
INSERT INTO `skill_list` VALUES ('LeechLife', '吸血', '吸取血液攻击对手，可以回复给予对手伤害3/4的HP');
INSERT INTO `skill_list` VALUES ('Liquidation', '水流裂破', '用水之力量猛击对手，有概率降低对手的防御');
INSERT INTO `skill_list` VALUES ('MaxSteelspike', '极巨钢铁', '钢属性攻击，提高自身防御');
INSERT INTO `skill_list` VALUES ('MegaDrain', '超级吸取', '可以回复给予对手一半伤害的HP');
INSERT INTO `skill_list` VALUES ('MegaPunch', '百万吨重拳', '用充满力量的拳头攻击对手');
INSERT INTO `skill_list` VALUES ('Outrage', '逆鳞', '乱打一气的进行攻击，自己下一回合无法行动');
INSERT INTO `skill_list` VALUES ('PoisonSting', '毒针', '将有毒的针刺入对手进行攻击。有时会让对手陷入中毒状态');
INSERT INTO `skill_list` VALUES ('Pound', '拍击', '使用长长的尾巴或手等拍打对手进行攻击');
INSERT INTO `skill_list` VALUES ('Psybeam', '幻象光线', '向对手发射神奇的光线进行攻击。有时会使对手混乱');
INSERT INTO `skill_list` VALUES ('QuickAttack', '电光一闪', '高速攻击，使用后提示自己的速度一回合');
INSERT INTO `skill_list` VALUES ('QuickGuard', '迂回', '若对方的速度快于自己，则免除伤害一回合');
INSERT INTO `skill_list` VALUES ('Reflect', '反射壁', '打开神奇的壁障，在几回合内减弱受到的伤害');
INSERT INTO `skill_list` VALUES ('SandAttack', '泼沙', '向对手脸上泼沙子，从而降低速度');
INSERT INTO `skill_list` VALUES ('SolarBlade', '日光刃', '蓄力一回合收集日光，下一回合进行强力攻击');
INSERT INTO `skill_list` VALUES ('Supersonic', '超音波', '从身体发出特殊的音波，从而使对手混乱');
INSERT INTO `skill_list` VALUES ('SurgingStrikes', '水流连打', '将水之流派修练至大成的仿若行云流水般的攻击，必定暴击');
INSERT INTO `skill_list` VALUES ('SwordsDance', '剑舞', '激烈地跳起战舞提高气势。大幅提高自己的攻击');

-- ----------------------------
-- Table structure for user__server
-- ----------------------------
DROP TABLE IF EXISTS `user__server`;
CREATE TABLE `user__server`  (
  `PKM_ID` int(0) NOT NULL AUTO_INCREMENT,
  `PKM_TYPEID` int(0) NOT NULL,
  `PKM_LEVEL` int(0) NOT NULL DEFAULT 1,
  `PKM_EXP` int(0) NOT NULL DEFAULT 0,
  `PKM_ATK` int(0) NOT NULL DEFAULT 0,
  `PKM_DEF` int(0) NOT NULL DEFAULT 0,
  `PKM_HP` int(0) NOT NULL DEFAULT 0,
  `PKM_SPD` int(0) NOT NULL DEFAULT 0,
  PRIMARY KEY (`PKM_ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 4 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of user__server
-- ----------------------------
INSERT INTO `user__server` VALUES (1, 815, 1, 0, 6, 6, 12, 6);
INSERT INTO `user__server` VALUES (2, 160, 4, 3, 16, 9, 24, 12);
INSERT INTO `user__server` VALUES (3, 160, 4, 3, 16, 9, 24, 12);

-- ----------------------------
-- Table structure for user_admin
-- ----------------------------
DROP TABLE IF EXISTS `user_admin`;
CREATE TABLE `user_admin`  (
  `PKM_ID` int(0) NOT NULL AUTO_INCREMENT,
  `PKM_TYPEID` int(0) NOT NULL,
  `PKM_LEVEL` int(0) NOT NULL DEFAULT 1,
  `PKM_EXP` int(0) NOT NULL DEFAULT 0,
  `PKM_ATK` int(0) NOT NULL DEFAULT 6,
  `PKM_DEF` int(0) NOT NULL DEFAULT 6,
  `PKM_HP` int(0) NOT NULL DEFAULT 12,
  `PKM_SPD` int(0) NOT NULL DEFAULT 6,
  PRIMARY KEY (`PKM_ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 6 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of user_admin
-- ----------------------------
INSERT INTO `user_admin` VALUES (1, 160, 7, 10, 25, 12, 36, 18);
INSERT INTO `user_admin` VALUES (2, 815, 6, 6, 16, 16, 27, 26);
INSERT INTO `user_admin` VALUES (3, 726, 1, 0, 6, 6, 12, 6);
INSERT INTO `user_admin` VALUES (5, 815, 1, 0, 6, 6, 12, 6);

-- ----------------------------
-- Table structure for user_dfw
-- ----------------------------
DROP TABLE IF EXISTS `user_dfw`;
CREATE TABLE `user_dfw`  (
  `PKM_ID` int(0) NOT NULL AUTO_INCREMENT,
  `PKM_TYPEID` int(0) NOT NULL,
  `PKM_LEVEL` int(0) NOT NULL DEFAULT 1,
  `PKM_EXP` int(0) NOT NULL DEFAULT 0,
  `PKM_ATK` int(0) NOT NULL DEFAULT 6,
  `PKM_DEF` int(0) NOT NULL DEFAULT 6,
  `PKM_HP` int(0) NOT NULL DEFAULT 12,
  `PKM_SPD` int(0) NOT NULL DEFAULT 6,
  PRIMARY KEY (`PKM_ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of user_dfw
-- ----------------------------
INSERT INTO `user_dfw` VALUES (1, 254, 1, 0, 6, 6, 12, 6);
INSERT INTO `user_dfw` VALUES (2, 812, 1, 0, 6, 6, 12, 6);
INSERT INTO `user_dfw` VALUES (3, 812, 1, 0, 6, 6, 12, 6);

-- ----------------------------
-- Table structure for user_list
-- ----------------------------
DROP TABLE IF EXISTS `user_list`;
CREATE TABLE `user_list`  (
  `USERNAME` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `PASSWORD` varchar(128) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `TOTAL_BATTLE_TIME` int(0) NOT NULL DEFAULT 0,
  `WINNER_TIME` int(0) NOT NULL DEFAULT 0,
  PRIMARY KEY (`USERNAME`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of user_list
-- ----------------------------
INSERT INTO `user_list` VALUES ('admin', 'admin', 19, 16);
INSERT INTO `user_list` VALUES ('DFW', 'password', 3, 0);
INSERT INTO `user_list` VALUES ('TestUser', 'password', 0, 0);

-- ----------------------------
-- Table structure for user_testuser
-- ----------------------------
DROP TABLE IF EXISTS `user_testuser`;
CREATE TABLE `user_testuser`  (
  `PKM_ID` int(0) NOT NULL AUTO_INCREMENT,
  `PKM_TYPEID` int(0) NOT NULL,
  `PKM_LEVEL` int(0) NOT NULL DEFAULT 1,
  `PKM_EXP` int(0) NOT NULL DEFAULT 0,
  `PKM_ATK` int(0) NOT NULL DEFAULT 6,
  `PKM_DEF` int(0) NOT NULL DEFAULT 6,
  `PKM_HP` int(0) NOT NULL DEFAULT 12,
  `PKM_SPD` int(0) NOT NULL DEFAULT 6,
  PRIMARY KEY (`PKM_ID`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of user_testuser
-- ----------------------------
INSERT INTO `user_testuser` VALUES (1, 254, 2, 0, 7, 9, 16, 8);
INSERT INTO `user_testuser` VALUES (2, 815, 1, 0, 6, 6, 12, 6);
INSERT INTO `user_testuser` VALUES (3, 160, 1, 0, 6, 6, 12, 6);

SET FOREIGN_KEY_CHECKS = 1;
