// begin MACVETH: headers added
#include <immintrin.h>
// end MACVETH
#include <RBio.h>
#include <xmmintrin.h>

#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

#define restrict __restrict

void kernel_spmv_fragment_0(double *restrict A, double *restrict x,
                            double *restrict y) {
  register int i0, i1, i2, i3, i4, i5, i6;
#pragma macveth unroll i0 full
  __m128d __mv_vop23, __mv_vop21, __mv_vop22, __mv_accm3, __mv_vop1525, __mv_vop29, __mv_vop27, __mv_vop28, __mv_accm5, __mv_vop1526, __mv_vop35, __mv_vop33, __mv_vop34, __mv_accm7, __mv_vop1527, __mv_vop41, __mv_vop39, __mv_vop40, __mv_accm9, __mv_vop1528, __mv_vop47, __mv_vop45, __mv_vop46, __mv_accm11, __mv_vop1529, __mv_vop53, __mv_vop51, __mv_vop52, __mv_accm13, __mv_vop1530, __mv_vop59, __mv_vop57, __mv_vop58, __mv_accm15, __mv_vop1531, __mv_vop65, __mv_vop63, __mv_vop64, __mv_accm17, __mv_vop1532, __mv_vop71, __mv_vop69, __mv_vop70, __mv_accm19, __mv_vop1533, __mv_vop77, __mv_vop75, __mv_vop76, __mv_accm21, __mv_vop1534, __mv_vop83, __mv_vop81, __mv_vop82, __mv_accm23, __mv_vop1535, __mv_vop89, __mv_vop87, __mv_vop88, __mv_accm25, __mv_vop1536, __mv_vop95, __mv_vop93, __mv_vop94, __mv_accm27, __mv_vop1537, __mv_vop101, __mv_vop99, __mv_vop100, __mv_accm29, __mv_vop1538, __mv_vop107, __mv_vop105, __mv_vop106, __mv_accm31, __mv_vop1539, __mv_vop113, __mv_vop111, __mv_vop112, __mv_accm33, __mv_vop1540, __mv_vop119, __mv_vop117, __mv_vop118, __mv_accm35, __mv_vop1541, __mv_vop125, __mv_vop123, __mv_vop124, __mv_accm37, __mv_vop1542, __mv_vop131, __mv_vop129, __mv_vop130, __mv_accm39, __mv_vop1543, __mv_vop137, __mv_vop135, __mv_vop136, __mv_accm41, __mv_vop1544, __mv_vop143, __mv_vop141, __mv_vop142, __mv_accm43, __mv_vop1545, __mv_vop149, __mv_vop147, __mv_vop148, __mv_accm45, __mv_vop1546, __mv_vop155, __mv_vop153, __mv_vop154, __mv_accm47, __mv_vop1547, __mv_vop161, __mv_vop159, __mv_vop160, __mv_accm49, __mv_vop1548, __mv_vop167, __mv_vop165, __mv_vop166, __mv_accm51, __mv_vop1549, __mv_vop173, __mv_vop171, __mv_vop172, __mv_accm53, __mv_vop1550, __mv_vop179, __mv_vop177, __mv_vop178, __mv_accm55, __mv_vop1551, __mv_vop185, __mv_vop183, __mv_vop184, __mv_accm57, __mv_vop1552, __mv_vop191, __mv_vop189, __mv_vop190, __mv_accm59, __mv_vop1553, __mv_vop197, __mv_vop195, __mv_vop196, __mv_accm61, __mv_vop1554, __mv_vop203, __mv_vop201, __mv_vop202, __mv_accm63, __mv_vop1555, __mv_vop209, __mv_vop207, __mv_vop208, __mv_accm65, __mv_vop1556, __mv_vop215, __mv_vop213, __mv_vop214, __mv_accm67, __mv_vop1557, __mv_vop221, __mv_vop219, __mv_vop220, __mv_accm69, __mv_vop1558, __mv_vop227, __mv_vop225, __mv_vop226, __mv_accm71, __mv_vop1559, __mv_vop233, __mv_vop231, __mv_vop232, __mv_accm73, __mv_vop1560, __mv_vop239, __mv_vop237, __mv_vop238, __mv_accm75, __mv_vop1561, __mv_vop245, __mv_vop243, __mv_vop244, __mv_accm77, __mv_vop1562, __mv_vop251, __mv_vop249, __mv_vop250, __mv_accm79, __mv_vop1563, __mv_vop257, __mv_vop255, __mv_vop256, __mv_accm81, __mv_vop1564, __mv_vop263, __mv_vop261, __mv_vop262, __mv_accm83, __mv_vop1565, __mv_vop269, __mv_vop267, __mv_vop268, __mv_accm85, __mv_vop1566, __mv_vop275, __mv_vop273, __mv_vop274, __mv_accm87, __mv_vop1567, __mv_vop281, __mv_vop279, __mv_vop280, __mv_accm89, __mv_vop1568, __mv_vop287, __mv_vop285, __mv_vop286, __mv_accm91, __mv_vop1569, __mv_vop293, __mv_vop291, __mv_vop292, __mv_accm93, __mv_vop1570, __mv_vop299, __mv_vop297, __mv_vop298, __mv_accm95, __mv_vop1571, __mv_vop305, __mv_vop303, __mv_vop304, __mv_accm97, __mv_vop1572, __mv_vop311, __mv_vop309, __mv_vop310, __mv_accm99, __mv_vop1573, __mv_vop317, __mv_vop315, __mv_vop316, __mv_accm101, __mv_vop1574, __mv_vop323, __mv_vop321, __mv_vop322, __mv_accm103, __mv_vop1575, __mv_vop329, __mv_vop327, __mv_vop328, __mv_accm105, __mv_vop1576, __mv_vop335, __mv_vop333, __mv_vop334, __mv_accm107, __mv_vop1577, __mv_vop341, __mv_vop339, __mv_vop340, __mv_accm109, __mv_vop1578, __mv_vop347, __mv_vop345, __mv_vop346, __mv_accm111, __mv_vop1579, __mv_vop353, __mv_vop351, __mv_vop352, __mv_accm113, __mv_vop1580, __mv_vop359, __mv_vop357, __mv_vop358, __mv_accm115, __mv_vop1581, __mv_vop365, __mv_vop363, __mv_vop364, __mv_accm117, __mv_vop1582, __mv_vop371, __mv_vop369, __mv_vop370, __mv_accm119, __mv_vop1583, __mv_vop377, __mv_vop375, __mv_vop376, __mv_accm121, __mv_vop1584, __mv_vop383, __mv_vop381, __mv_vop382, __mv_accm123, __mv_vop1585, __mv_vop389, __mv_vop387, __mv_vop388, __mv_accm125, __mv_vop1586, __mv_vop395, __mv_vop393, __mv_vop394, __mv_accm127, __mv_vop1587, __mv_vop401, __mv_vop399, __mv_vop400, __mv_accm129, __mv_vop1588, __mv_vop407, __mv_vop405, __mv_vop406, __mv_accm131, __mv_vop1589, __mv_vop413, __mv_vop411, __mv_vop412, __mv_accm133, __mv_vop1590, __mv_vop419, __mv_vop417, __mv_vop418, __mv_accm135, __mv_vop1591, __mv_vop425, __mv_vop423, __mv_vop424, __mv_accm137, __mv_vop1592, __mv_vop431, __mv_vop429, __mv_vop430, __mv_accm139, __mv_vop1593, __mv_vop437, __mv_vop435, __mv_vop436, __mv_accm141, __mv_vop1594, __mv_vop443, __mv_vop441, __mv_vop442, __mv_accm143, __mv_vop1595, __mv_vop449, __mv_vop447, __mv_vop448, __mv_accm145, __mv_vop1596, __mv_vop455, __mv_vop453, __mv_vop454, __mv_accm147, __mv_vop1597, __mv_vop461, __mv_vop459, __mv_vop460, __mv_accm149, __mv_vop1598, __mv_vop467, __mv_vop465, __mv_vop466, __mv_accm151, __mv_vop1599, __mv_vop473, __mv_vop471, __mv_vop472, __mv_accm153, __mv_vop1600, __mv_vop479, __mv_vop477, __mv_vop478, __mv_accm155, __mv_vop1601, __mv_vop485, __mv_vop483, __mv_vop484, __mv_accm157, __mv_vop1602, __mv_vop491, __mv_vop489, __mv_vop490, __mv_accm159, __mv_vop1603, __mv_vop503, __mv_vop501, __mv_vop502, __mv_accm163, __mv_vop1604, __mv_vop509, __mv_vop507, __mv_vop508, __mv_accm165, __mv_vop1605, __mv_vop515, __mv_vop513, __mv_vop514, __mv_accm167, __mv_vop1606, __mv_vop521, __mv_vop519, __mv_vop520, __mv_accm169, __mv_vop1607, __mv_vop527, __mv_vop525, __mv_vop526, __mv_accm171, __mv_vop1608, __mv_vop533, __mv_vop531, __mv_vop532, __mv_accm173, __mv_vop1609, __mv_vop539, __mv_vop537, __mv_vop538, __mv_accm175, __mv_vop1610, __mv_vop545, __mv_vop543, __mv_vop544, __mv_accm177, __mv_vop1611, __mv_vop551, __mv_vop549, __mv_vop550, __mv_accm179, __mv_vop1612, __mv_vop557, __mv_vop555, __mv_vop556, __mv_accm181, __mv_vop1613, __mv_vop563, __mv_vop561, __mv_vop562, __mv_accm183, __mv_vop1614, __mv_vop569, __mv_vop567, __mv_vop568, __mv_accm185, __mv_vop1615, __mv_vop575, __mv_vop573, __mv_vop574, __mv_accm187, __mv_vop1616, __mv_vop581, __mv_vop579, __mv_vop580, __mv_accm189, __mv_vop1617, __mv_vop587, __mv_vop585, __mv_vop586, __mv_accm191, __mv_vop1618, __mv_vop593, __mv_vop591, __mv_vop592, __mv_accm193, __mv_vop1619, __mv_vop599, __mv_vop597, __mv_vop598, __mv_accm195, __mv_vop1620, __mv_vop605, __mv_vop603, __mv_vop604, __mv_accm197, __mv_vop1621, __mv_vop611, __mv_vop609, __mv_vop610, __mv_accm199, __mv_vop1622, __mv_vop617, __mv_vop615, __mv_vop616, __mv_accm201, __mv_vop1623, __mv_vop623, __mv_vop621, __mv_vop622, __mv_accm203, __mv_vop1624, __mv_vop629, __mv_vop627, __mv_vop628, __mv_accm205, __mv_vop1625, __mv_vop635, __mv_vop633, __mv_vop634, __mv_accm207, __mv_vop1626, __mv_vop641, __mv_vop639, __mv_vop640, __mv_accm209, __mv_vop1627, __mv_vop647, __mv_vop645, __mv_vop646, __mv_accm211, __mv_vop1628, __mv_vop653, __mv_vop651, __mv_vop652, __mv_accm213, __mv_vop1629, __mv_vop659, __mv_vop657, __mv_vop658, __mv_accm215, __mv_vop1630, __mv_vop665, __mv_vop663, __mv_vop664, __mv_accm217, __mv_vop1631, __mv_vop671, __mv_vop669, __mv_vop670, __mv_accm219, __mv_vop1632, __mv_vop677, __mv_vop675, __mv_vop676, __mv_accm221, __mv_vop1633, __mv_vop683, __mv_vop681, __mv_vop682, __mv_accm223, __mv_vop1634, __mv_vop689, __mv_vop687, __mv_vop688, __mv_accm225, __mv_vop1635, __mv_vop695, __mv_vop693, __mv_vop694, __mv_accm227, __mv_vop1636, __mv_vop701, __mv_vop699, __mv_vop700, __mv_accm229, __mv_vop1637, __mv_vop707, __mv_vop705, __mv_vop706, __mv_accm231, __mv_vop1638, __mv_vop713, __mv_vop711, __mv_vop712, __mv_accm233, __mv_vop1639, __mv_vop719, __mv_vop717, __mv_vop718, __mv_accm235, __mv_vop1640, __mv_vop725, __mv_vop723, __mv_vop724, __mv_accm237, __mv_vop1641, __mv_vop731, __mv_vop729, __mv_vop730, __mv_accm239, __mv_vop1642, __mv_vop737, __mv_vop735, __mv_vop736, __mv_accm241, __mv_vop1643, __mv_vop743, __mv_vop741, __mv_vop742, __mv_accm243, __mv_vop1644, __mv_vop749, __mv_vop747, __mv_vop748, __mv_accm245, __mv_vop1645, __mv_vop755, __mv_vop753, __mv_vop754, __mv_accm247, __mv_vop1646, __mv_vop761, __mv_vop759, __mv_vop760, __mv_accm249, __mv_vop1647, __mv_vop767, __mv_vop765, __mv_vop766, __mv_accm251, __mv_vop1648, __mv_vop773, __mv_vop771, __mv_vop772, __mv_accm253, __mv_vop1649, __mv_vop779, __mv_vop777, __mv_vop778, __mv_accm255, __mv_vop1650, __mv_vop785, __mv_vop783, __mv_vop784, __mv_accm257, __mv_vop1651, __mv_vop791, __mv_vop789, __mv_vop790, __mv_accm259, __mv_vop1652, __mv_vop797, __mv_vop795, __mv_vop796, __mv_accm261, __mv_vop1653, __mv_vop803, __mv_vop801, __mv_vop802, __mv_accm263, __mv_vop1654, __mv_vop809, __mv_vop807, __mv_vop808, __mv_accm265, __mv_vop1655, __mv_vop815, __mv_vop813, __mv_vop814, __mv_accm267, __mv_vop1656, __mv_vop821, __mv_vop819, __mv_vop820, __mv_accm269, __mv_vop1657, __mv_vop827, __mv_vop825, __mv_vop826, __mv_accm271, __mv_vop1658, __mv_vop833, __mv_vop831, __mv_vop832, __mv_accm273, __mv_vop1659, __mv_vop839, __mv_vop837, __mv_vop838, __mv_accm275, __mv_vop1660, __mv_vop845, __mv_vop843, __mv_vop844, __mv_accm277, __mv_vop1661, __mv_vop851, __mv_vop849, __mv_vop850, __mv_accm279, __mv_vop1662, __mv_vop857, __mv_vop855, __mv_vop856, __mv_accm281, __mv_vop1663, __mv_vop863, __mv_vop861, __mv_vop862, __mv_accm283, __mv_vop1664, __mv_vop869, __mv_vop867, __mv_vop868, __mv_accm285, __mv_vop1665, __mv_vop875, __mv_vop873, __mv_vop874, __mv_accm287, __mv_vop1666, __mv_vop881, __mv_vop879, __mv_vop880, __mv_accm289, __mv_vop1667, __mv_vop887, __mv_vop885, __mv_vop886, __mv_accm291, __mv_vop1668, __mv_vop893, __mv_vop891, __mv_vop892, __mv_accm293, __mv_vop1669, __mv_vop899, __mv_vop897, __mv_vop898, __mv_accm295, __mv_vop1670, __mv_vop905, __mv_vop903, __mv_vop904, __mv_accm297, __mv_vop1671, __mv_vop911, __mv_vop909, __mv_vop910, __mv_accm299, __mv_vop1672, __mv_vop917, __mv_vop915, __mv_vop916, __mv_accm301, __mv_vop1673, __mv_vop923, __mv_vop921, __mv_vop922, __mv_accm303, __mv_vop1674, __mv_vop929, __mv_vop927, __mv_vop928, __mv_accm305, __mv_vop1675, __mv_vop935, __mv_vop933, __mv_vop934, __mv_accm307, __mv_vop1676, __mv_vop941, __mv_vop939, __mv_vop940, __mv_accm309, __mv_vop1677, __mv_vop947, __mv_vop945, __mv_vop946, __mv_accm311, __mv_vop1678, __mv_vop953, __mv_vop951, __mv_vop952, __mv_accm313, __mv_vop1679, __mv_vop959, __mv_vop957, __mv_vop958, __mv_accm315, __mv_vop1680, __mv_vop965, __mv_vop963, __mv_vop964, __mv_accm317, __mv_vop1681, __mv_vop971, __mv_vop969, __mv_vop970, __mv_accm319, __mv_vop1682, __mv_vop980, __mv_vop978, __mv_vop979, __mv_accm322, __mv_vop1683, __mv_vop986, __mv_vop984, __mv_vop985, __mv_accm324, __mv_vop1684, __mv_vop992, __mv_vop990, __mv_vop991, __mv_accm326, __mv_vop1685, __mv_vop998, __mv_vop996, __mv_vop997, __mv_accm328, __mv_vop1686, __mv_vop1004, __mv_vop1002, __mv_vop1003, __mv_accm330, __mv_vop1687, __mv_vop1010, __mv_vop1008, __mv_vop1009, __mv_accm332, __mv_vop1688, __mv_vop1016, __mv_vop1014, __mv_vop1015, __mv_accm334, __mv_vop1689, __mv_vop1022, __mv_vop1020, __mv_vop1021, __mv_accm336, __mv_vop1690, __mv_vop1028, __mv_vop1026, __mv_vop1027, __mv_accm338, __mv_vop1691, __mv_vop1034, __mv_vop1032, __mv_vop1033, __mv_accm340, __mv_vop1692, __mv_vop1040, __mv_vop1038, __mv_vop1039, __mv_accm342, __mv_vop1693, __mv_vop1046, __mv_vop1044, __mv_vop1045, __mv_accm344, __mv_vop1694, __mv_vop1052, __mv_vop1050, __mv_vop1051, __mv_accm346, __mv_vop1695, __mv_vop1058, __mv_vop1056, __mv_vop1057, __mv_accm348, __mv_vop1696, __mv_vop1064, __mv_vop1062, __mv_vop1063, __mv_accm350, __mv_vop1697, __mv_vop1070, __mv_vop1068, __mv_vop1069, __mv_accm352, __mv_vop1698, __mv_vop1076, __mv_vop1074, __mv_vop1075, __mv_accm354, __mv_vop1699, __mv_vop1082, __mv_vop1080, __mv_vop1081, __mv_accm356, __mv_vop1700, __mv_vop1088, __mv_vop1086, __mv_vop1087, __mv_accm358, __mv_vop1701, __mv_vop1094, __mv_vop1092, __mv_vop1093, __mv_accm360, __mv_vop1702, __mv_vop1100, __mv_vop1098, __mv_vop1099, __mv_accm362, __mv_vop1703, __mv_vop1106, __mv_vop1104, __mv_vop1105, __mv_accm364, __mv_vop1704, __mv_vop1112, __mv_vop1110, __mv_vop1111, __mv_accm366, __mv_vop1705, __mv_vop1118, __mv_vop1116, __mv_vop1117, __mv_accm368, __mv_vop1706, __mv_vop1124, __mv_vop1122, __mv_vop1123, __mv_accm370, __mv_vop1707, __mv_vop1130, __mv_vop1128, __mv_vop1129, __mv_accm372, __mv_vop1708, __mv_vop1136, __mv_vop1134, __mv_vop1135, __mv_accm374, __mv_vop1709, __mv_vop1142, __mv_vop1140, __mv_vop1141, __mv_accm376, __mv_vop1710, __mv_vop1148, __mv_vop1146, __mv_vop1147, __mv_accm378, __mv_vop1711, __mv_vop1154, __mv_vop1152, __mv_vop1153, __mv_accm380, __mv_vop1712, __mv_vop1160, __mv_vop1158, __mv_vop1159, __mv_accm382, __mv_vop1713, __mv_vop1166, __mv_vop1164, __mv_vop1165, __mv_accm384, __mv_vop1714, __mv_vop1172, __mv_vop1170, __mv_vop1171, __mv_accm386, __mv_vop1715, __mv_vop1178, __mv_vop1176, __mv_vop1177, __mv_accm388, __mv_vop1716, __mv_vop1184, __mv_vop1182, __mv_vop1183, __mv_accm390, __mv_vop1717, __mv_vop1190, __mv_vop1188, __mv_vop1189, __mv_accm392, __mv_vop1718, __mv_vop1196, __mv_vop1194, __mv_vop1195, __mv_accm394, __mv_vop1719, __mv_vop1202, __mv_vop1200, __mv_vop1201, __mv_accm396, __mv_vop1720, __mv_vop1208, __mv_vop1206, __mv_vop1207, __mv_accm398, __mv_vop1721, __mv_vop1214, __mv_vop1212, __mv_vop1213, __mv_accm400, __mv_vop1722, __mv_vop1220, __mv_vop1218, __mv_vop1219, __mv_accm402, __mv_vop1723, __mv_vop1226, __mv_vop1224, __mv_vop1225, __mv_accm404, __mv_vop1724, __mv_vop1232, __mv_vop1230, __mv_vop1231, __mv_accm406, __mv_vop1725, __mv_vop1238, __mv_vop1236, __mv_vop1237, __mv_accm408, __mv_vop1726, __mv_vop1244, __mv_vop1242, __mv_vop1243, __mv_accm410, __mv_vop1727, __mv_vop1250, __mv_vop1248, __mv_vop1249, __mv_accm412, __mv_vop1728, __mv_vop1256, __mv_vop1254, __mv_vop1255, __mv_accm414, __mv_vop1729, __mv_vop1262, __mv_vop1260, __mv_vop1261, __mv_accm416, __mv_vop1730, __mv_vop1268, __mv_vop1266, __mv_vop1267, __mv_accm418, __mv_vop1731, __mv_vop1274, __mv_vop1272, __mv_vop1273, __mv_accm420, __mv_vop1732, __mv_vop1280, __mv_vop1278, __mv_vop1279, __mv_accm422, __mv_vop1733, __mv_vop1286, __mv_vop1284, __mv_vop1285, __mv_accm424, __mv_vop1734, __mv_vop1292, __mv_vop1290, __mv_vop1291, __mv_accm426, __mv_vop1735, __mv_vop1298, __mv_vop1296, __mv_vop1297, __mv_accm428, __mv_vop1736, __mv_vop1304, __mv_vop1302, __mv_vop1303, __mv_accm430, __mv_vop1737, __mv_vop1310, __mv_vop1308, __mv_vop1309, __mv_accm432, __mv_vop1738, __mv_vop1316, __mv_vop1314, __mv_vop1315, __mv_accm434, __mv_vop1739, __mv_vop1322, __mv_vop1320, __mv_vop1321, __mv_accm436, __mv_vop1740, __mv_vop1328, __mv_vop1326, __mv_vop1327, __mv_accm438, __mv_vop1741, __mv_vop1334, __mv_vop1332, __mv_vop1333, __mv_accm440, __mv_vop1742, __mv_vop1340, __mv_vop1338, __mv_vop1339, __mv_accm442, __mv_vop1743, __mv_vop1346, __mv_vop1344, __mv_vop1345, __mv_accm444, __mv_vop1744, __mv_vop1352, __mv_vop1350, __mv_vop1351, __mv_accm446, __mv_vop1745, __mv_vop1358, __mv_vop1356, __mv_vop1357, __mv_accm448, __mv_vop1746, __mv_vop1364, __mv_vop1362, __mv_vop1363, __mv_accm450, __mv_vop1747, __mv_vop1370, __mv_vop1368, __mv_vop1369, __mv_accm452, __mv_vop1748, __mv_vop1376, __mv_vop1374, __mv_vop1375, __mv_accm454, __mv_vop1749, __mv_vop1382, __mv_vop1380, __mv_vop1381, __mv_accm456, __mv_vop1750, __mv_vop1388, __mv_vop1386, __mv_vop1387, __mv_accm458, __mv_vop1751, __mv_vop1394, __mv_vop1392, __mv_vop1393, __mv_accm460, __mv_vop1752, __mv_vop1400, __mv_vop1398, __mv_vop1399, __mv_accm462, __mv_vop1753, __mv_vop1406, __mv_vop1404, __mv_vop1405, __mv_accm464, __mv_vop1754, __mv_vop1412, __mv_vop1410, __mv_vop1411, __mv_accm466, __mv_vop1755, __mv_vop1418, __mv_vop1416, __mv_vop1417, __mv_accm468, __mv_vop1756, __mv_vop1424, __mv_vop1422, __mv_vop1423, __mv_accm470, __mv_vop1757, __mv_vop1430, __mv_vop1428, __mv_vop1429, __mv_accm472, __mv_vop1758, __mv_vop1436, __mv_vop1434, __mv_vop1435, __mv_accm474, __mv_vop1759, __mv_vop1442, __mv_vop1440, __mv_vop1441, __mv_accm476, __mv_vop1760, __mv_vop1448, __mv_vop1446, __mv_vop1447, __mv_accm478, __mv_vop1761, __mv_vop1454, __mv_vop1452, __mv_vop1453, __mv_accm480, __mv_vop1762, __mv_vop1457, __mv_vop1455, __mv_vop1456, __mv_accm481, __mv_vop1763, __mv_vop1460, __mv_vop1458, __mv_vop1459, __mv_accm482, __mv_vop1764, __mv_vop1463, __mv_vop1461, __mv_vop1462, __mv_accm483, __mv_vop1765, __mv_vop1469, __mv_vop1467, __mv_vop1468, __mv_accm485, __mv_vop1766, __mv_vop1475, __mv_vop1473, __mv_vop1474, __mv_accm487, __mv_vop1767, __mv_vop1481, __mv_vop1479, __mv_vop1480, __mv_accm489, __mv_vop1768, __mv_vop1487, __mv_vop1485, __mv_vop1486, __mv_accm491, __mv_vop1769, __mv_vop1493, __mv_vop1491, __mv_vop1492, __mv_accm493, __mv_vop1770, __mv_vop1499, __mv_vop1497, __mv_vop1498, __mv_accm495, __mv_vop1771, __mv_vop1505, __mv_vop1503, __mv_vop1504, __mv_accm497, __mv_vop1772, __mv_vop1511, __mv_vop1509, __mv_vop1510, __mv_accm499, __mv_vop1773, __mv_vop1517, __mv_vop1515, __mv_vop1516, __mv_accm501, __mv_vop1774, __mv_lo256, __mv_hi256, __mv_lo, __mv_hi;
  __m256d __mv_vop2, __mv_vop0, __mv_vop1, __mv_vop1521, __mv_vop5, __mv_vop3, __mv_vop4, __mv_accm0, __mv_vop1775, __mv_vop8, __mv_vop6, __mv_vop7, __mv_vop1522, __mv_vop11, __mv_vop9, __mv_vop10, __mv_vop1523, __mv_vop14, __mv_vop12, __mv_vop13, __mv_vop1524, __mv_vop17, __mv_vop15, __mv_vop16, __mv_accm1, __mv_vop1776, __mv_vop20, __mv_vop18, __mv_vop19, __mv_accm2, __mv_vop1777, __mv_vop26, __mv_vop24, __mv_vop25, __mv_accm4, __mv_vop1778, __mv_vop32, __mv_vop30, __mv_vop31, __mv_accm6, __mv_vop1779, __mv_vop38, __mv_vop36, __mv_vop37, __mv_accm8, __mv_vop1780, __mv_vop44, __mv_vop42, __mv_vop43, __mv_accm10, __mv_vop1781, __mv_vop50, __mv_vop48, __mv_vop49, __mv_accm12, __mv_vop1782, __mv_vop56, __mv_vop54, __mv_vop55, __mv_accm14, __mv_vop1783, __mv_vop62, __mv_vop60, __mv_vop61, __mv_accm16, __mv_vop1784, __mv_vop68, __mv_vop66, __mv_vop67, __mv_accm18, __mv_vop1785, __mv_vop74, __mv_vop72, __mv_vop73, __mv_accm20, __mv_vop1786, __mv_vop80, __mv_vop78, __mv_vop79, __mv_accm22, __mv_vop1787, __mv_vop86, __mv_vop84, __mv_vop85, __mv_accm24, __mv_vop1788, __mv_vop92, __mv_vop90, __mv_vop91, __mv_accm26, __mv_vop1789, __mv_vop98, __mv_vop96, __mv_vop97, __mv_accm28, __mv_vop1790, __mv_vop104, __mv_vop102, __mv_vop103, __mv_accm30, __mv_vop1791, __mv_vop110, __mv_vop108, __mv_vop109, __mv_accm32, __mv_vop1792, __mv_vop116, __mv_vop114, __mv_vop115, __mv_accm34, __mv_vop1793, __mv_vop122, __mv_vop120, __mv_vop121, __mv_accm36, __mv_vop1794, __mv_vop128, __mv_vop126, __mv_vop127, __mv_accm38, __mv_vop1795, __mv_vop134, __mv_vop132, __mv_vop133, __mv_accm40, __mv_vop1796, __mv_vop140, __mv_vop138, __mv_vop139, __mv_accm42, __mv_vop1797, __mv_vop146, __mv_vop144, __mv_vop145, __mv_accm44, __mv_vop1798, __mv_vop152, __mv_vop150, __mv_vop151, __mv_accm46, __mv_vop1799, __mv_vop158, __mv_vop156, __mv_vop157, __mv_accm48, __mv_vop1800, __mv_vop164, __mv_vop162, __mv_vop163, __mv_accm50, __mv_vop1801, __mv_vop170, __mv_vop168, __mv_vop169, __mv_accm52, __mv_vop1802, __mv_vop176, __mv_vop174, __mv_vop175, __mv_accm54, __mv_vop1803, __mv_vop182, __mv_vop180, __mv_vop181, __mv_accm56, __mv_vop1804, __mv_vop188, __mv_vop186, __mv_vop187, __mv_accm58, __mv_vop1805, __mv_vop194, __mv_vop192, __mv_vop193, __mv_accm60, __mv_vop1806, __mv_vop200, __mv_vop198, __mv_vop199, __mv_accm62, __mv_vop1807, __mv_vop206, __mv_vop204, __mv_vop205, __mv_accm64, __mv_vop1808, __mv_vop212, __mv_vop210, __mv_vop211, __mv_accm66, __mv_vop1809, __mv_vop218, __mv_vop216, __mv_vop217, __mv_accm68, __mv_vop1810, __mv_vop224, __mv_vop222, __mv_vop223, __mv_accm70, __mv_vop1811, __mv_vop230, __mv_vop228, __mv_vop229, __mv_accm72, __mv_vop1812, __mv_vop236, __mv_vop234, __mv_vop235, __mv_accm74, __mv_vop1813, __mv_vop242, __mv_vop240, __mv_vop241, __mv_accm76, __mv_vop1814, __mv_vop248, __mv_vop246, __mv_vop247, __mv_accm78, __mv_vop1815, __mv_vop254, __mv_vop252, __mv_vop253, __mv_accm80, __mv_vop1816, __mv_vop260, __mv_vop258, __mv_vop259, __mv_accm82, __mv_vop1817, __mv_vop266, __mv_vop264, __mv_vop265, __mv_accm84, __mv_vop1818, __mv_vop272, __mv_vop270, __mv_vop271, __mv_accm86, __mv_vop1819, __mv_vop278, __mv_vop276, __mv_vop277, __mv_accm88, __mv_vop1820, __mv_vop284, __mv_vop282, __mv_vop283, __mv_accm90, __mv_vop1821, __mv_vop290, __mv_vop288, __mv_vop289, __mv_accm92, __mv_vop1822, __mv_vop296, __mv_vop294, __mv_vop295, __mv_accm94, __mv_vop1823, __mv_vop302, __mv_vop300, __mv_vop301, __mv_accm96, __mv_vop1824, __mv_vop308, __mv_vop306, __mv_vop307, __mv_accm98, __mv_vop1825, __mv_vop314, __mv_vop312, __mv_vop313, __mv_accm100, __mv_vop1826, __mv_vop320, __mv_vop318, __mv_vop319, __mv_accm102, __mv_vop1827, __mv_vop326, __mv_vop324, __mv_vop325, __mv_accm104, __mv_vop1828, __mv_vop332, __mv_vop330, __mv_vop331, __mv_accm106, __mv_vop1829, __mv_vop338, __mv_vop336, __mv_vop337, __mv_accm108, __mv_vop1830, __mv_vop344, __mv_vop342, __mv_vop343, __mv_accm110, __mv_vop1831, __mv_vop350, __mv_vop348, __mv_vop349, __mv_accm112, __mv_vop1832, __mv_vop356, __mv_vop354, __mv_vop355, __mv_accm114, __mv_vop1833, __mv_vop362, __mv_vop360, __mv_vop361, __mv_accm116, __mv_vop1834, __mv_vop368, __mv_vop366, __mv_vop367, __mv_accm118, __mv_vop1835, __mv_vop374, __mv_vop372, __mv_vop373, __mv_accm120, __mv_vop1836, __mv_vop380, __mv_vop378, __mv_vop379, __mv_accm122, __mv_vop1837, __mv_vop386, __mv_vop384, __mv_vop385, __mv_accm124, __mv_vop1838, __mv_vop392, __mv_vop390, __mv_vop391, __mv_accm126, __mv_vop1839, __mv_vop398, __mv_vop396, __mv_vop397, __mv_accm128, __mv_vop1840, __mv_vop404, __mv_vop402, __mv_vop403, __mv_accm130, __mv_vop1841, __mv_vop410, __mv_vop408, __mv_vop409, __mv_accm132, __mv_vop1842, __mv_vop416, __mv_vop414, __mv_vop415, __mv_accm134, __mv_vop1843, __mv_vop422, __mv_vop420, __mv_vop421, __mv_accm136, __mv_vop1844, __mv_vop428, __mv_vop426, __mv_vop427, __mv_accm138, __mv_vop1845, __mv_vop434, __mv_vop432, __mv_vop433, __mv_accm140, __mv_vop1846, __mv_vop440, __mv_vop438, __mv_vop439, __mv_accm142, __mv_vop1847, __mv_vop446, __mv_vop444, __mv_vop445, __mv_accm144, __mv_vop1848, __mv_vop452, __mv_vop450, __mv_vop451, __mv_accm146, __mv_vop1849, __mv_vop458, __mv_vop456, __mv_vop457, __mv_accm148, __mv_vop1850, __mv_vop464, __mv_vop462, __mv_vop463, __mv_accm150, __mv_vop1851, __mv_vop470, __mv_vop468, __mv_vop469, __mv_accm152, __mv_vop1852, __mv_vop476, __mv_vop474, __mv_vop475, __mv_accm154, __mv_vop1853, __mv_vop482, __mv_vop480, __mv_vop481, __mv_accm156, __mv_vop1854, __mv_vop488, __mv_vop486, __mv_vop487, __mv_accm158, __mv_vop1855, __mv_vop494, __mv_vop492, __mv_vop493, __mv_accm160, __mv_vop1856, __mv_vop497, __mv_vop495, __mv_vop496, __mv_accm161, __mv_vop2026, __mv_vop500, __mv_vop498, __mv_vop499, __mv_accm162, __mv_vop1857, __mv_vop506, __mv_vop504, __mv_vop505, __mv_accm164, __mv_vop1858, __mv_vop512, __mv_vop510, __mv_vop511, __mv_accm166, __mv_vop1859, __mv_vop518, __mv_vop516, __mv_vop517, __mv_accm168, __mv_vop1860, __mv_vop524, __mv_vop522, __mv_vop523, __mv_accm170, __mv_vop1861, __mv_vop530, __mv_vop528, __mv_vop529, __mv_accm172, __mv_vop1862, __mv_vop536, __mv_vop534, __mv_vop535, __mv_accm174, __mv_vop1863, __mv_vop542, __mv_vop540, __mv_vop541, __mv_accm176, __mv_vop1864, __mv_vop548, __mv_vop546, __mv_vop547, __mv_accm178, __mv_vop1865, __mv_vop554, __mv_vop552, __mv_vop553, __mv_accm180, __mv_vop1866, __mv_vop560, __mv_vop558, __mv_vop559, __mv_accm182, __mv_vop1867, __mv_vop566, __mv_vop564, __mv_vop565, __mv_accm184, __mv_vop1868, __mv_vop572, __mv_vop570, __mv_vop571, __mv_accm186, __mv_vop1869, __mv_vop578, __mv_vop576, __mv_vop577, __mv_accm188, __mv_vop1870, __mv_vop584, __mv_vop582, __mv_vop583, __mv_accm190, __mv_vop1871, __mv_vop590, __mv_vop588, __mv_vop589, __mv_accm192, __mv_vop1872, __mv_vop596, __mv_vop594, __mv_vop595, __mv_accm194, __mv_vop1873, __mv_vop602, __mv_vop600, __mv_vop601, __mv_accm196, __mv_vop1874, __mv_vop608, __mv_vop606, __mv_vop607, __mv_accm198, __mv_vop1875, __mv_vop614, __mv_vop612, __mv_vop613, __mv_accm200, __mv_vop1876, __mv_vop620, __mv_vop618, __mv_vop619, __mv_accm202, __mv_vop1877, __mv_vop626, __mv_vop624, __mv_vop625, __mv_accm204, __mv_vop1878, __mv_vop632, __mv_vop630, __mv_vop631, __mv_accm206, __mv_vop1879, __mv_vop638, __mv_vop636, __mv_vop637, __mv_accm208, __mv_vop1880, __mv_vop644, __mv_vop642, __mv_vop643, __mv_accm210, __mv_vop1881, __mv_vop650, __mv_vop648, __mv_vop649, __mv_accm212, __mv_vop1882, __mv_vop656, __mv_vop654, __mv_vop655, __mv_accm214, __mv_vop1883, __mv_vop662, __mv_vop660, __mv_vop661, __mv_accm216, __mv_vop1884, __mv_vop668, __mv_vop666, __mv_vop667, __mv_accm218, __mv_vop1885, __mv_vop674, __mv_vop672, __mv_vop673, __mv_accm220, __mv_vop1886, __mv_vop680, __mv_vop678, __mv_vop679, __mv_accm222, __mv_vop1887, __mv_vop686, __mv_vop684, __mv_vop685, __mv_accm224, __mv_vop1888, __mv_vop692, __mv_vop690, __mv_vop691, __mv_accm226, __mv_vop1889, __mv_vop698, __mv_vop696, __mv_vop697, __mv_accm228, __mv_vop1890, __mv_vop704, __mv_vop702, __mv_vop703, __mv_accm230, __mv_vop1891, __mv_vop710, __mv_vop708, __mv_vop709, __mv_accm232, __mv_vop1892, __mv_vop716, __mv_vop714, __mv_vop715, __mv_accm234, __mv_vop1893, __mv_vop722, __mv_vop720, __mv_vop721, __mv_accm236, __mv_vop1894, __mv_vop728, __mv_vop726, __mv_vop727, __mv_accm238, __mv_vop1895, __mv_vop734, __mv_vop732, __mv_vop733, __mv_accm240, __mv_vop1896, __mv_vop740, __mv_vop738, __mv_vop739, __mv_accm242, __mv_vop1897, __mv_vop746, __mv_vop744, __mv_vop745, __mv_accm244, __mv_vop1898, __mv_vop752, __mv_vop750, __mv_vop751, __mv_accm246, __mv_vop1899, __mv_vop758, __mv_vop756, __mv_vop757, __mv_accm248, __mv_vop1900, __mv_vop764, __mv_vop762, __mv_vop763, __mv_accm250, __mv_vop1901, __mv_vop770, __mv_vop768, __mv_vop769, __mv_accm252, __mv_vop1902, __mv_vop776, __mv_vop774, __mv_vop775, __mv_accm254, __mv_vop1903, __mv_vop782, __mv_vop780, __mv_vop781, __mv_accm256, __mv_vop1904, __mv_vop788, __mv_vop786, __mv_vop787, __mv_accm258, __mv_vop1905, __mv_vop794, __mv_vop792, __mv_vop793, __mv_accm260, __mv_vop1906, __mv_vop800, __mv_vop798, __mv_vop799, __mv_accm262, __mv_vop1907, __mv_vop806, __mv_vop804, __mv_vop805, __mv_accm264, __mv_vop1908, __mv_vop812, __mv_vop810, __mv_vop811, __mv_accm266, __mv_vop1909, __mv_vop818, __mv_vop816, __mv_vop817, __mv_accm268, __mv_vop1910, __mv_vop824, __mv_vop822, __mv_vop823, __mv_accm270, __mv_vop1911, __mv_vop830, __mv_vop828, __mv_vop829, __mv_accm272, __mv_vop1912, __mv_vop836, __mv_vop834, __mv_vop835, __mv_accm274, __mv_vop1913, __mv_vop842, __mv_vop840, __mv_vop841, __mv_accm276, __mv_vop1914, __mv_vop848, __mv_vop846, __mv_vop847, __mv_accm278, __mv_vop1915, __mv_vop854, __mv_vop852, __mv_vop853, __mv_accm280, __mv_vop1916, __mv_vop860, __mv_vop858, __mv_vop859, __mv_accm282, __mv_vop1917, __mv_vop866, __mv_vop864, __mv_vop865, __mv_accm284, __mv_vop1918, __mv_vop872, __mv_vop870, __mv_vop871, __mv_accm286, __mv_vop1919, __mv_vop878, __mv_vop876, __mv_vop877, __mv_accm288, __mv_vop1920, __mv_vop884, __mv_vop882, __mv_vop883, __mv_accm290, __mv_vop1921, __mv_vop890, __mv_vop888, __mv_vop889, __mv_accm292, __mv_vop1922, __mv_vop896, __mv_vop894, __mv_vop895, __mv_accm294, __mv_vop1923, __mv_vop902, __mv_vop900, __mv_vop901, __mv_accm296, __mv_vop1924, __mv_vop908, __mv_vop906, __mv_vop907, __mv_accm298, __mv_vop1925, __mv_vop914, __mv_vop912, __mv_vop913, __mv_accm300, __mv_vop1926, __mv_vop920, __mv_vop918, __mv_vop919, __mv_accm302, __mv_vop1927, __mv_vop926, __mv_vop924, __mv_vop925, __mv_accm304, __mv_vop1928, __mv_vop932, __mv_vop930, __mv_vop931, __mv_accm306, __mv_vop1929, __mv_vop938, __mv_vop936, __mv_vop937, __mv_accm308, __mv_vop1930, __mv_vop944, __mv_vop942, __mv_vop943, __mv_accm310, __mv_vop1931, __mv_vop950, __mv_vop948, __mv_vop949, __mv_accm312, __mv_vop1932, __mv_vop956, __mv_vop954, __mv_vop955, __mv_accm314, __mv_vop1933, __mv_vop962, __mv_vop960, __mv_vop961, __mv_accm316, __mv_vop1934, __mv_vop968, __mv_vop966, __mv_vop967, __mv_accm318, __mv_vop1935, __mv_vop974, __mv_vop972, __mv_vop973, __mv_accm320, __mv_vop1936, __mv_vop977, __mv_vop975, __mv_vop976, __mv_accm321, __mv_vop2027, __mv_vop983, __mv_vop981, __mv_vop982, __mv_accm323, __mv_vop1937, __mv_vop989, __mv_vop987, __mv_vop988, __mv_accm325, __mv_vop1938, __mv_vop995, __mv_vop993, __mv_vop994, __mv_accm327, __mv_vop1939, __mv_vop1001, __mv_vop999, __mv_vop1000, __mv_accm329, __mv_vop1940, __mv_vop1007, __mv_vop1005, __mv_vop1006, __mv_accm331, __mv_vop1941, __mv_vop1013, __mv_vop1011, __mv_vop1012, __mv_accm333, __mv_vop1942, __mv_vop1019, __mv_vop1017, __mv_vop1018, __mv_accm335, __mv_vop1943, __mv_vop1025, __mv_vop1023, __mv_vop1024, __mv_accm337, __mv_vop1944, __mv_vop1031, __mv_vop1029, __mv_vop1030, __mv_accm339, __mv_vop1945, __mv_vop1037, __mv_vop1035, __mv_vop1036, __mv_accm341, __mv_vop1946, __mv_vop1043, __mv_vop1041, __mv_vop1042, __mv_accm343, __mv_vop1947, __mv_vop1049, __mv_vop1047, __mv_vop1048, __mv_accm345, __mv_vop1948, __mv_vop1055, __mv_vop1053, __mv_vop1054, __mv_accm347, __mv_vop1949, __mv_vop1061, __mv_vop1059, __mv_vop1060, __mv_accm349, __mv_vop1950, __mv_vop1067, __mv_vop1065, __mv_vop1066, __mv_accm351, __mv_vop1951, __mv_vop1073, __mv_vop1071, __mv_vop1072, __mv_accm353, __mv_vop1952, __mv_vop1079, __mv_vop1077, __mv_vop1078, __mv_accm355, __mv_vop1953, __mv_vop1085, __mv_vop1083, __mv_vop1084, __mv_accm357, __mv_vop1954, __mv_vop1091, __mv_vop1089, __mv_vop1090, __mv_accm359, __mv_vop1955, __mv_vop1097, __mv_vop1095, __mv_vop1096, __mv_accm361, __mv_vop1956, __mv_vop1103, __mv_vop1101, __mv_vop1102, __mv_accm363, __mv_vop1957, __mv_vop1109, __mv_vop1107, __mv_vop1108, __mv_accm365, __mv_vop1958, __mv_vop1115, __mv_vop1113, __mv_vop1114, __mv_accm367, __mv_vop1959, __mv_vop1121, __mv_vop1119, __mv_vop1120, __mv_accm369, __mv_vop1960, __mv_vop1127, __mv_vop1125, __mv_vop1126, __mv_accm371, __mv_vop1961, __mv_vop1133, __mv_vop1131, __mv_vop1132, __mv_accm373, __mv_vop1962, __mv_vop1139, __mv_vop1137, __mv_vop1138, __mv_accm375, __mv_vop1963, __mv_vop1145, __mv_vop1143, __mv_vop1144, __mv_accm377, __mv_vop1964, __mv_vop1151, __mv_vop1149, __mv_vop1150, __mv_accm379, __mv_vop1965, __mv_vop1157, __mv_vop1155, __mv_vop1156, __mv_accm381, __mv_vop1966, __mv_vop1163, __mv_vop1161, __mv_vop1162, __mv_accm383, __mv_vop1967, __mv_vop1169, __mv_vop1167, __mv_vop1168, __mv_accm385, __mv_vop1968, __mv_vop1175, __mv_vop1173, __mv_vop1174, __mv_accm387, __mv_vop1969, __mv_vop1181, __mv_vop1179, __mv_vop1180, __mv_accm389, __mv_vop1970, __mv_vop1187, __mv_vop1185, __mv_vop1186, __mv_accm391, __mv_vop1971, __mv_vop1193, __mv_vop1191, __mv_vop1192, __mv_accm393, __mv_vop1972, __mv_vop1199, __mv_vop1197, __mv_vop1198, __mv_accm395, __mv_vop1973, __mv_vop1205, __mv_vop1203, __mv_vop1204, __mv_accm397, __mv_vop1974, __mv_vop1211, __mv_vop1209, __mv_vop1210, __mv_accm399, __mv_vop1975, __mv_vop1217, __mv_vop1215, __mv_vop1216, __mv_accm401, __mv_vop1976, __mv_vop1223, __mv_vop1221, __mv_vop1222, __mv_accm403, __mv_vop1977, __mv_vop1229, __mv_vop1227, __mv_vop1228, __mv_accm405, __mv_vop1978, __mv_vop1235, __mv_vop1233, __mv_vop1234, __mv_accm407, __mv_vop1979, __mv_vop1241, __mv_vop1239, __mv_vop1240, __mv_accm409, __mv_vop1980, __mv_vop1247, __mv_vop1245, __mv_vop1246, __mv_accm411, __mv_vop1981, __mv_vop1253, __mv_vop1251, __mv_vop1252, __mv_accm413, __mv_vop1982, __mv_vop1259, __mv_vop1257, __mv_vop1258, __mv_accm415, __mv_vop1983, __mv_vop1265, __mv_vop1263, __mv_vop1264, __mv_accm417, __mv_vop1984, __mv_vop1271, __mv_vop1269, __mv_vop1270, __mv_accm419, __mv_vop1985, __mv_vop1277, __mv_vop1275, __mv_vop1276, __mv_accm421, __mv_vop1986, __mv_vop1283, __mv_vop1281, __mv_vop1282, __mv_accm423, __mv_vop1987, __mv_vop1289, __mv_vop1287, __mv_vop1288, __mv_accm425, __mv_vop1988, __mv_vop1295, __mv_vop1293, __mv_vop1294, __mv_accm427, __mv_vop1989, __mv_vop1301, __mv_vop1299, __mv_vop1300, __mv_accm429, __mv_vop1990, __mv_vop1307, __mv_vop1305, __mv_vop1306, __mv_accm431, __mv_vop1991, __mv_vop1313, __mv_vop1311, __mv_vop1312, __mv_accm433, __mv_vop1992, __mv_vop1319, __mv_vop1317, __mv_vop1318, __mv_accm435, __mv_vop1993, __mv_vop1325, __mv_vop1323, __mv_vop1324, __mv_accm437, __mv_vop1994, __mv_vop1331, __mv_vop1329, __mv_vop1330, __mv_accm439, __mv_vop1995, __mv_vop1337, __mv_vop1335, __mv_vop1336, __mv_accm441, __mv_vop1996, __mv_vop1343, __mv_vop1341, __mv_vop1342, __mv_accm443, __mv_vop1997, __mv_vop1349, __mv_vop1347, __mv_vop1348, __mv_accm445, __mv_vop1998, __mv_vop1355, __mv_vop1353, __mv_vop1354, __mv_accm447, __mv_vop1999, __mv_vop1361, __mv_vop1359, __mv_vop1360, __mv_accm449, __mv_vop2000, __mv_vop1367, __mv_vop1365, __mv_vop1366, __mv_accm451, __mv_vop2001, __mv_vop1373, __mv_vop1371, __mv_vop1372, __mv_accm453, __mv_vop2002, __mv_vop1379, __mv_vop1377, __mv_vop1378, __mv_accm455, __mv_vop2003, __mv_vop1385, __mv_vop1383, __mv_vop1384, __mv_accm457, __mv_vop2004, __mv_vop1391, __mv_vop1389, __mv_vop1390, __mv_accm459, __mv_vop2005, __mv_vop1397, __mv_vop1395, __mv_vop1396, __mv_accm461, __mv_vop2006, __mv_vop1403, __mv_vop1401, __mv_vop1402, __mv_accm463, __mv_vop2007, __mv_vop1409, __mv_vop1407, __mv_vop1408, __mv_accm465, __mv_vop2008, __mv_vop1415, __mv_vop1413, __mv_vop1414, __mv_accm467, __mv_vop2009, __mv_vop1421, __mv_vop1419, __mv_vop1420, __mv_accm469, __mv_vop2010, __mv_vop1427, __mv_vop1425, __mv_vop1426, __mv_accm471, __mv_vop2011, __mv_vop1433, __mv_vop1431, __mv_vop1432, __mv_accm473, __mv_vop2012, __mv_vop1439, __mv_vop1437, __mv_vop1438, __mv_accm475, __mv_vop2013, __mv_vop1445, __mv_vop1443, __mv_vop1444, __mv_accm477, __mv_vop2014, __mv_vop1451, __mv_vop1449, __mv_vop1450, __mv_accm479, __mv_vop2015, __mv_vop1466, __mv_vop1464, __mv_vop1465, __mv_accm484, __mv_vop2016, __mv_vop1472, __mv_vop1470, __mv_vop1471, __mv_accm486, __mv_vop2017, __mv_vop1478, __mv_vop1476, __mv_vop1477, __mv_accm488, __mv_vop2018, __mv_vop1484, __mv_vop1482, __mv_vop1483, __mv_accm490, __mv_vop2019, __mv_vop1490, __mv_vop1488, __mv_vop1489, __mv_accm492, __mv_vop2020, __mv_vop1496, __mv_vop1494, __mv_vop1495, __mv_accm494, __mv_vop2021, __mv_vop1502, __mv_vop1500, __mv_vop1501, __mv_accm496, __mv_vop2022, __mv_vop1508, __mv_vop1506, __mv_vop1507, __mv_accm498, __mv_vop2023, __mv_vop1514, __mv_vop1512, __mv_vop1513, __mv_accm500, __mv_vop2024, __mv_vop1520, __mv_vop1518, __mv_vop1519, __mv_accm502, __mv_vop2025;
  __mv_vop0 = _mm256_loadu_pd(&A[0]);	// latency = 1
  __mv_vop1 = _mm256_i64gather_pd(&x[14], _mm256_set_epi64x(3998, 320, 160, 0), 8);	// latency = 10
  __mv_vop1521 = _mm256_loadu_pd(&y[0]);	// latency = 1
  __mv_vop1521 = _mm256_fmadd_pd(__mv_vop0, __mv_vop1, __mv_vop1521);	// latency = 5
  _mm256_storeu_pd(&y[0], __mv_vop1521);	// latency = 1
  // stmt vectorized: y[0] += A[0] * x[14];
  // stmt vectorized: y[1] += A[1] * x[174];
  // stmt vectorized: y[2] += A[2] * x[334];
  // stmt vectorized: y[3] += A[3] * x[4012];
  __mv_vop3 = _mm256_maskload_pd(&A[4], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
  __mv_vop4 = _mm256_mask_i64gather_pd(_mm256_setzero_pd(), &x[5], _mm256_set_epi64x(0,3368, 1447, 0), _mm256_set_pd(0,-1,-1,-1), 8);	// latency = 10
  __mv_accm0 = _mm256_fmadd_pd(__mv_vop3, __mv_vop4, __mv_accm0);	// latency = 5
  // stmt vectorized: y[5] += A[4] * x[5]
__mv_lo256 = _mm256_castpd256_pd128(__mv_accm0);
__mv_hi256 = _mm256_extractf128_pd(__mv_accm0, 0x1);
__mv_lo256 = _mm_add_pd(__mv_lo256, __mv_hi256);
__mv_hi256 = _mm_shuffle_pd(__mv_lo256, __mv_lo256, 0x1);
__mv_lo256 = _mm_add_pd(__mv_lo256, __mv_hi256);
y[5] = y[5]+__mv_lo256[0];;
  // stmt vectorized: y[5] += A[5] * x[1452];
  // stmt vectorized: y[5] += A[6] * x[3373];
  for (i0 = 0; i0 <= 4; i0 += 5) {
    __mv_vop6 = _mm256_i64gather_pd(&A[((2 * (i0 + 0)) + 7)], _mm256_set_epi64x(6, 4, 2, 0), 8);	// latency = 10
    __mv_vop7 = _mm256_loadu_pd(&x[((i0 + 0) + 6)]);	// latency = 1
    __mv_vop1522 = _mm256_loadu_pd(&y[((i0 + 0) + 6)]);	// latency = 1
    __mv_vop1522 = _mm256_fmadd_pd(__mv_vop6, __mv_vop7, __mv_vop1522);	// latency = 5
    _mm256_storeu_pd(&y[((i0 + 0) + 6)], __mv_vop1522);	// latency = 1
    y[((i0 + 4) + 6)] = (y[((i0 + 4) + 6)] + (A[((2 * (i0 + 4)) + 7)] * x[((i0 + 4) + 6)]));
    // stmt not vectorized: y[1 * i0 + 6] += A[2 * i0 + 7] * x[1 * i0 + 6];
  }
  __mv_vop9 = _mm256_i64gather_pd(&A[8], _mm256_set_epi64x(6, 4, 2, 0), 8);	// latency = 10
  __mv_vop10 = _mm256_i64gather_pd(&x[1452], _mm256_set_epi64x(1279, 960, 320, 0), 8);	// latency = 10
  __mv_vop1523 = _mm256_loadu_pd(&y[6]);	// latency = 1
  __mv_vop1523 = _mm256_fmadd_pd(__mv_vop9, __mv_vop10, __mv_vop1523);	// latency = 5
  _mm256_storeu_pd(&y[6], __mv_vop1523);	// latency = 1
  // stmt vectorized: y[6] += A[8] * x[1452];
  // stmt vectorized: y[7] += A[10] * x[1772];
  // stmt vectorized: y[8] += A[12] * x[2412];
  // stmt vectorized: y[9] += A[14] * x[2731];
  __mv_vop12 = _mm256_mask_i64gather_pd(_mm256_setzero_pd(), &A[16], _mm256_set_epi64x(0,4, 2, 0), _mm256_set_pd(0,-1,-1,-1), 8);	// latency = 10
  __mv_vop13 = _mm256_mask_i64gather_pd(_mm256_setzero_pd(), &x[3212], _mm256_set_epi64x(0,161, 321, 0), _mm256_set_pd(0,-1,-1,-1), 8);	// latency = 10
  __mv_vop1524 = _mm256_maskload_pd(&y[10], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
  __mv_vop1524 = _mm256_fmadd_pd(__mv_vop12, __mv_vop13, __mv_vop1524);	// latency = 5
  _mm256_maskstore_pd(&y[10], _mm256_set_epi64x(0,-1,-1,-1), __mv_vop1524);	// latency = 1
  // stmt vectorized: y[10] += A[16] * x[3212];
  // stmt vectorized: y[11] += A[18] * x[3533];
  // stmt vectorized: y[12] += A[20] * x[3373];
  __mv_vop15 = _mm256_maskload_pd(&A[22], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
  __mv_vop16 = _mm256_mask_i64gather_pd(_mm256_setzero_pd(), &x[0], _mm256_set_epi64x(0,4013, 15, 0), _mm256_set_pd(0,-1,-1,-1), 8);	// latency = 10
  __mv_accm1 = _mm256_fmadd_pd(__mv_vop15, __mv_vop16, __mv_accm1);	// latency = 5
  // stmt vectorized: y[14] += A[22] * x[0]
__mv_lo256 = _mm256_castpd256_pd128(__mv_accm1);
__mv_hi256 = _mm256_extractf128_pd(__mv_accm1, 0x1);
__mv_lo256 = _mm_add_pd(__mv_lo256, __mv_hi256);
__mv_hi256 = _mm_shuffle_pd(__mv_lo256, __mv_lo256, 0x1);
__mv_lo256 = _mm_add_pd(__mv_lo256, __mv_hi256);
y[14] = y[14]+__mv_lo256[0];;
  // stmt vectorized: y[14] += A[23] * x[15];
  // stmt vectorized: y[14] += A[24] * x[4013];

  __mv_accm2 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop18 = _mm256_maskload_pd(&A[((i0 + 0) + 25)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop19 = _mm256_maskload_pd(&x[((i0 + 0) + 14)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm2 = _mm256_fmadd_pd(__mv_vop18, __mv_vop19, __mv_accm2);	// latency = 5
    // stmt vectorized: y[0 * i0 + 15] += A[1 * i0 + 25] * x[1 * i0 + 14];
  }

  __mv_vop21 = _mm_loadu_pd(&A[28]);	// latency = 1
  __mv_vop22 = _mm_i64gather_pd(&x[15], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm3 = _mm_fmadd_pd(__mv_vop21, __mv_vop22, __mv_accm3);	// latency = 5
  // stmt vectorized: y[16] += A[28] * x[15]
__mv_hi = _mm_shuffle_pd(__mv_accm3, __mv_accm3, 0x1);
__mv_accm3 = _mm_add_pd(__mv_accm3, __mv_hi);
y[16] = y[16]+__mv_accm3[0];;
  // stmt vectorized: y[16] += A[29] * x[17];
  __mv_accm4 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop24 = _mm256_maskload_pd(&A[((i0 + 0) + 30)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop25 = _mm256_maskload_pd(&x[((i0 + 0) + 16)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm4 = _mm256_fmadd_pd(__mv_vop24, __mv_vop25, __mv_accm4);	// latency = 5
    // stmt vectorized: y[0 * i0 + 17] += A[1 * i0 + 30] * x[1 * i0 + 16];
  }
  __mv_vop27 = _mm_loadu_pd(&A[33]);	// latency = 1
  __mv_vop28 = _mm_i64gather_pd(&x[17], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm5 = _mm_fmadd_pd(__mv_vop27, __mv_vop28, __mv_accm5);	// latency = 5
  // stmt vectorized: y[18] += A[33] * x[17]
__mv_hi = _mm_shuffle_pd(__mv_accm5, __mv_accm5, 0x1);
__mv_accm5 = _mm_add_pd(__mv_accm5, __mv_hi);
y[18] = y[18]+__mv_accm5[0];;
  // stmt vectorized: y[18] += A[34] * x[19];
  __mv_accm6 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop30 = _mm256_maskload_pd(&A[((i0 + 0) + 35)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop31 = _mm256_maskload_pd(&x[((i0 + 0) + 18)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm6 = _mm256_fmadd_pd(__mv_vop30, __mv_vop31, __mv_accm6);	// latency = 5
    // stmt vectorized: y[0 * i0 + 19] += A[1 * i0 + 35] * x[1 * i0 + 18];
  }
  __mv_vop33 = _mm_loadu_pd(&A[38]);	// latency = 1
  __mv_vop34 = _mm_i64gather_pd(&x[19], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm7 = _mm_fmadd_pd(__mv_vop33, __mv_vop34, __mv_accm7);	// latency = 5
  // stmt vectorized: y[20] += A[38] * x[19]
__mv_hi = _mm_shuffle_pd(__mv_accm7, __mv_accm7, 0x1);
__mv_accm7 = _mm_add_pd(__mv_accm7, __mv_hi);
y[20] = y[20]+__mv_accm7[0];;
  // stmt vectorized: y[20] += A[39] * x[21];
  __mv_accm8 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop36 = _mm256_maskload_pd(&A[((i0 + 0) + 40)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop37 = _mm256_maskload_pd(&x[((i0 + 0) + 20)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm8 = _mm256_fmadd_pd(__mv_vop36, __mv_vop37, __mv_accm8);	// latency = 5
    // stmt vectorized: y[0 * i0 + 21] += A[1 * i0 + 40] * x[1 * i0 + 20];
  }__mv_accm2 = _mm256_hadd_pd(__mv_accm2, __mv_accm4);	// latency = 6
__mv_accm2 = _mm256_add_pd(_mm256_blend_pd(__mv_accm2,__mv_accm6,0b1100), _mm256_permute2f128_pd(__mv_accm2,__mv_accm6,0x21));	// latency = 4
y[15] = y[15] + __mv_accm2[0];	// latency = 1
y[17] = y[17] + __mv_accm2[1];	// latency = 1
y[19] = y[19] + __mv_accm2[2];	// latency = 1
y[21] = y[21] + __mv_accm2[3];	// latency = 1

  __mv_vop39 = _mm_loadu_pd(&A[43]);	// latency = 1
  __mv_vop40 = _mm_i64gather_pd(&x[21], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm9 = _mm_fmadd_pd(__mv_vop39, __mv_vop40, __mv_accm9);	// latency = 5
  // stmt vectorized: y[22] += A[43] * x[21]
__mv_hi = _mm_shuffle_pd(__mv_accm9, __mv_accm9, 0x1);
__mv_accm9 = _mm_add_pd(__mv_accm9, __mv_hi);
y[22] = y[22]+__mv_accm9[0];;
  // stmt vectorized: y[22] += A[44] * x[23];
  __mv_accm10 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop42 = _mm256_maskload_pd(&A[((i0 + 0) + 45)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop43 = _mm256_maskload_pd(&x[((i0 + 0) + 22)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm10 = _mm256_fmadd_pd(__mv_vop42, __mv_vop43, __mv_accm10);	// latency = 5
    // stmt vectorized: y[0 * i0 + 23] += A[1 * i0 + 45] * x[1 * i0 + 22];
  }
  __mv_vop45 = _mm_loadu_pd(&A[48]);	// latency = 1
  __mv_vop46 = _mm_i64gather_pd(&x[23], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm11 = _mm_fmadd_pd(__mv_vop45, __mv_vop46, __mv_accm11);	// latency = 5
  // stmt vectorized: y[24] += A[48] * x[23]
__mv_hi = _mm_shuffle_pd(__mv_accm11, __mv_accm11, 0x1);
__mv_accm11 = _mm_add_pd(__mv_accm11, __mv_hi);
y[24] = y[24]+__mv_accm11[0];;
  // stmt vectorized: y[24] += A[49] * x[25];
  __mv_accm12 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop48 = _mm256_maskload_pd(&A[((i0 + 0) + 50)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop49 = _mm256_maskload_pd(&x[((i0 + 0) + 24)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm12 = _mm256_fmadd_pd(__mv_vop48, __mv_vop49, __mv_accm12);	// latency = 5
    // stmt vectorized: y[0 * i0 + 25] += A[1 * i0 + 50] * x[1 * i0 + 24];
  }
  __mv_vop51 = _mm_loadu_pd(&A[53]);	// latency = 1
  __mv_vop52 = _mm_i64gather_pd(&x[25], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm13 = _mm_fmadd_pd(__mv_vop51, __mv_vop52, __mv_accm13);	// latency = 5
  // stmt vectorized: y[26] += A[53] * x[25]
__mv_hi = _mm_shuffle_pd(__mv_accm13, __mv_accm13, 0x1);
__mv_accm13 = _mm_add_pd(__mv_accm13, __mv_hi);
y[26] = y[26]+__mv_accm13[0];;
  // stmt vectorized: y[26] += A[54] * x[27];
  __mv_accm14 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop54 = _mm256_maskload_pd(&A[((i0 + 0) + 55)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop55 = _mm256_maskload_pd(&x[((i0 + 0) + 26)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm14 = _mm256_fmadd_pd(__mv_vop54, __mv_vop55, __mv_accm14);	// latency = 5
    // stmt vectorized: y[0 * i0 + 27] += A[1 * i0 + 55] * x[1 * i0 + 26];
  }
  __mv_vop57 = _mm_loadu_pd(&A[58]);	// latency = 1
  __mv_vop58 = _mm_i64gather_pd(&x[27], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm15 = _mm_fmadd_pd(__mv_vop57, __mv_vop58, __mv_accm15);	// latency = 5
  // stmt vectorized: y[28] += A[58] * x[27]
__mv_hi = _mm_shuffle_pd(__mv_accm15, __mv_accm15, 0x1);
__mv_accm15 = _mm_add_pd(__mv_accm15, __mv_hi);
y[28] = y[28]+__mv_accm15[0];;
  // stmt vectorized: y[28] += A[59] * x[29];
  __mv_accm16 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop60 = _mm256_maskload_pd(&A[((i0 + 0) + 60)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop61 = _mm256_maskload_pd(&x[((i0 + 0) + 28)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm16 = _mm256_fmadd_pd(__mv_vop60, __mv_vop61, __mv_accm16);	// latency = 5
    // stmt vectorized: y[0 * i0 + 29] += A[1 * i0 + 60] * x[1 * i0 + 28];
  }__mv_accm10 = _mm256_hadd_pd(__mv_accm10, __mv_accm12);	// latency = 6
__mv_accm10 = _mm256_add_pd(_mm256_blend_pd(__mv_accm10,__mv_accm14,0b1100), _mm256_permute2f128_pd(__mv_accm10,__mv_accm14,0x21));	// latency = 4
y[23] = y[23] + __mv_accm10[0];	// latency = 1
y[25] = y[25] + __mv_accm10[1];	// latency = 1
y[27] = y[27] + __mv_accm10[2];	// latency = 1
y[29] = y[29] + __mv_accm10[3];	// latency = 1

  __mv_vop63 = _mm_loadu_pd(&A[63]);	// latency = 1
  __mv_vop64 = _mm_i64gather_pd(&x[29], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm17 = _mm_fmadd_pd(__mv_vop63, __mv_vop64, __mv_accm17);	// latency = 5
  // stmt vectorized: y[30] += A[63] * x[29]
__mv_hi = _mm_shuffle_pd(__mv_accm17, __mv_accm17, 0x1);
__mv_accm17 = _mm_add_pd(__mv_accm17, __mv_hi);
y[30] = y[30]+__mv_accm17[0];;
  // stmt vectorized: y[30] += A[64] * x[31];
  __mv_accm18 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop66 = _mm256_maskload_pd(&A[((i0 + 0) + 65)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop67 = _mm256_maskload_pd(&x[((i0 + 0) + 30)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm18 = _mm256_fmadd_pd(__mv_vop66, __mv_vop67, __mv_accm18);	// latency = 5
    // stmt vectorized: y[0 * i0 + 31] += A[1 * i0 + 65] * x[1 * i0 + 30];
  }
  __mv_vop69 = _mm_loadu_pd(&A[68]);	// latency = 1
  __mv_vop70 = _mm_i64gather_pd(&x[31], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm19 = _mm_fmadd_pd(__mv_vop69, __mv_vop70, __mv_accm19);	// latency = 5
  // stmt vectorized: y[32] += A[68] * x[31]
__mv_hi = _mm_shuffle_pd(__mv_accm19, __mv_accm19, 0x1);
__mv_accm19 = _mm_add_pd(__mv_accm19, __mv_hi);
y[32] = y[32]+__mv_accm19[0];;
  // stmt vectorized: y[32] += A[69] * x[33];
  __mv_accm20 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop72 = _mm256_maskload_pd(&A[((i0 + 0) + 70)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop73 = _mm256_maskload_pd(&x[((i0 + 0) + 32)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm20 = _mm256_fmadd_pd(__mv_vop72, __mv_vop73, __mv_accm20);	// latency = 5
    // stmt vectorized: y[0 * i0 + 33] += A[1 * i0 + 70] * x[1 * i0 + 32];
  }
  __mv_vop75 = _mm_loadu_pd(&A[73]);	// latency = 1
  __mv_vop76 = _mm_i64gather_pd(&x[33], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm21 = _mm_fmadd_pd(__mv_vop75, __mv_vop76, __mv_accm21);	// latency = 5
  // stmt vectorized: y[34] += A[73] * x[33]
__mv_hi = _mm_shuffle_pd(__mv_accm21, __mv_accm21, 0x1);
__mv_accm21 = _mm_add_pd(__mv_accm21, __mv_hi);
y[34] = y[34]+__mv_accm21[0];;
  // stmt vectorized: y[34] += A[74] * x[35];
  __mv_accm22 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop78 = _mm256_maskload_pd(&A[((i0 + 0) + 75)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop79 = _mm256_maskload_pd(&x[((i0 + 0) + 34)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm22 = _mm256_fmadd_pd(__mv_vop78, __mv_vop79, __mv_accm22);	// latency = 5
    // stmt vectorized: y[0 * i0 + 35] += A[1 * i0 + 75] * x[1 * i0 + 34];
  }
  __mv_vop81 = _mm_loadu_pd(&A[78]);	// latency = 1
  __mv_vop82 = _mm_i64gather_pd(&x[35], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm23 = _mm_fmadd_pd(__mv_vop81, __mv_vop82, __mv_accm23);	// latency = 5
  // stmt vectorized: y[36] += A[78] * x[35]
__mv_hi = _mm_shuffle_pd(__mv_accm23, __mv_accm23, 0x1);
__mv_accm23 = _mm_add_pd(__mv_accm23, __mv_hi);
y[36] = y[36]+__mv_accm23[0];;
  // stmt vectorized: y[36] += A[79] * x[37];
  __mv_accm24 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop84 = _mm256_maskload_pd(&A[((i0 + 0) + 80)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop85 = _mm256_maskload_pd(&x[((i0 + 0) + 36)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm24 = _mm256_fmadd_pd(__mv_vop84, __mv_vop85, __mv_accm24);	// latency = 5
    // stmt vectorized: y[0 * i0 + 37] += A[1 * i0 + 80] * x[1 * i0 + 36];
  }__mv_accm18 = _mm256_hadd_pd(__mv_accm18, __mv_accm20);	// latency = 6
__mv_accm18 = _mm256_add_pd(_mm256_blend_pd(__mv_accm18,__mv_accm22,0b1100), _mm256_permute2f128_pd(__mv_accm18,__mv_accm22,0x21));	// latency = 4
y[31] = y[31] + __mv_accm18[0];	// latency = 1
y[33] = y[33] + __mv_accm18[1];	// latency = 1
y[35] = y[35] + __mv_accm18[2];	// latency = 1
y[37] = y[37] + __mv_accm18[3];	// latency = 1

  __mv_vop87 = _mm_loadu_pd(&A[83]);	// latency = 1
  __mv_vop88 = _mm_i64gather_pd(&x[37], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm25 = _mm_fmadd_pd(__mv_vop87, __mv_vop88, __mv_accm25);	// latency = 5
  // stmt vectorized: y[38] += A[83] * x[37]
__mv_hi = _mm_shuffle_pd(__mv_accm25, __mv_accm25, 0x1);
__mv_accm25 = _mm_add_pd(__mv_accm25, __mv_hi);
y[38] = y[38]+__mv_accm25[0];;
  // stmt vectorized: y[38] += A[84] * x[39];
  __mv_accm26 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop90 = _mm256_maskload_pd(&A[((i0 + 0) + 85)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop91 = _mm256_maskload_pd(&x[((i0 + 0) + 38)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm26 = _mm256_fmadd_pd(__mv_vop90, __mv_vop91, __mv_accm26);	// latency = 5
    // stmt vectorized: y[0 * i0 + 39] += A[1 * i0 + 85] * x[1 * i0 + 38];
  }
  __mv_vop93 = _mm_loadu_pd(&A[88]);	// latency = 1
  __mv_vop94 = _mm_i64gather_pd(&x[39], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm27 = _mm_fmadd_pd(__mv_vop93, __mv_vop94, __mv_accm27);	// latency = 5
  // stmt vectorized: y[40] += A[88] * x[39]
__mv_hi = _mm_shuffle_pd(__mv_accm27, __mv_accm27, 0x1);
__mv_accm27 = _mm_add_pd(__mv_accm27, __mv_hi);
y[40] = y[40]+__mv_accm27[0];;
  // stmt vectorized: y[40] += A[89] * x[41];
  __mv_accm28 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop96 = _mm256_maskload_pd(&A[((i0 + 0) + 90)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop97 = _mm256_maskload_pd(&x[((i0 + 0) + 40)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm28 = _mm256_fmadd_pd(__mv_vop96, __mv_vop97, __mv_accm28);	// latency = 5
    // stmt vectorized: y[0 * i0 + 41] += A[1 * i0 + 90] * x[1 * i0 + 40];
  }
  __mv_vop99 = _mm_loadu_pd(&A[93]);	// latency = 1
  __mv_vop100 = _mm_i64gather_pd(&x[41], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm29 = _mm_fmadd_pd(__mv_vop99, __mv_vop100, __mv_accm29);	// latency = 5
  // stmt vectorized: y[42] += A[93] * x[41]
__mv_hi = _mm_shuffle_pd(__mv_accm29, __mv_accm29, 0x1);
__mv_accm29 = _mm_add_pd(__mv_accm29, __mv_hi);
y[42] = y[42]+__mv_accm29[0];;
  // stmt vectorized: y[42] += A[94] * x[43];
  __mv_accm30 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop102 = _mm256_maskload_pd(&A[((i0 + 0) + 95)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop103 = _mm256_maskload_pd(&x[((i0 + 0) + 42)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm30 = _mm256_fmadd_pd(__mv_vop102, __mv_vop103, __mv_accm30);	// latency = 5
    // stmt vectorized: y[0 * i0 + 43] += A[1 * i0 + 95] * x[1 * i0 + 42];
  }
  __mv_vop105 = _mm_loadu_pd(&A[98]);	// latency = 1
  __mv_vop106 = _mm_i64gather_pd(&x[43], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm31 = _mm_fmadd_pd(__mv_vop105, __mv_vop106, __mv_accm31);	// latency = 5
  // stmt vectorized: y[44] += A[98] * x[43]
__mv_hi = _mm_shuffle_pd(__mv_accm31, __mv_accm31, 0x1);
__mv_accm31 = _mm_add_pd(__mv_accm31, __mv_hi);
y[44] = y[44]+__mv_accm31[0];;
  // stmt vectorized: y[44] += A[99] * x[45];
  __mv_accm32 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop108 = _mm256_maskload_pd(&A[((i0 + 0) + 100)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop109 = _mm256_maskload_pd(&x[((i0 + 0) + 44)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm32 = _mm256_fmadd_pd(__mv_vop108, __mv_vop109, __mv_accm32);	// latency = 5
    // stmt vectorized: y[0 * i0 + 45] += A[1 * i0 + 100] * x[1 * i0 + 44];
  }__mv_accm26 = _mm256_hadd_pd(__mv_accm26, __mv_accm28);	// latency = 6
__mv_accm26 = _mm256_add_pd(_mm256_blend_pd(__mv_accm26,__mv_accm30,0b1100), _mm256_permute2f128_pd(__mv_accm26,__mv_accm30,0x21));	// latency = 4
y[39] = y[39] + __mv_accm26[0];	// latency = 1
y[41] = y[41] + __mv_accm26[1];	// latency = 1
y[43] = y[43] + __mv_accm26[2];	// latency = 1
y[45] = y[45] + __mv_accm26[3];	// latency = 1

  __mv_vop111 = _mm_loadu_pd(&A[103]);	// latency = 1
  __mv_vop112 = _mm_i64gather_pd(&x[45], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm33 = _mm_fmadd_pd(__mv_vop111, __mv_vop112, __mv_accm33);	// latency = 5
  // stmt vectorized: y[46] += A[103] * x[45]
__mv_hi = _mm_shuffle_pd(__mv_accm33, __mv_accm33, 0x1);
__mv_accm33 = _mm_add_pd(__mv_accm33, __mv_hi);
y[46] = y[46]+__mv_accm33[0];;
  // stmt vectorized: y[46] += A[104] * x[47];
  __mv_accm34 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop114 = _mm256_maskload_pd(&A[((i0 + 0) + 105)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop115 = _mm256_maskload_pd(&x[((i0 + 0) + 46)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm34 = _mm256_fmadd_pd(__mv_vop114, __mv_vop115, __mv_accm34);	// latency = 5
    // stmt vectorized: y[0 * i0 + 47] += A[1 * i0 + 105] * x[1 * i0 + 46];
  }
  __mv_vop117 = _mm_loadu_pd(&A[108]);	// latency = 1
  __mv_vop118 = _mm_i64gather_pd(&x[47], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm35 = _mm_fmadd_pd(__mv_vop117, __mv_vop118, __mv_accm35);	// latency = 5
  // stmt vectorized: y[48] += A[108] * x[47]
__mv_hi = _mm_shuffle_pd(__mv_accm35, __mv_accm35, 0x1);
__mv_accm35 = _mm_add_pd(__mv_accm35, __mv_hi);
y[48] = y[48]+__mv_accm35[0];;
  // stmt vectorized: y[48] += A[109] * x[49];
  __mv_accm36 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop120 = _mm256_maskload_pd(&A[((i0 + 0) + 110)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop121 = _mm256_maskload_pd(&x[((i0 + 0) + 48)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm36 = _mm256_fmadd_pd(__mv_vop120, __mv_vop121, __mv_accm36);	// latency = 5
    // stmt vectorized: y[0 * i0 + 49] += A[1 * i0 + 110] * x[1 * i0 + 48];
  }
  __mv_vop123 = _mm_loadu_pd(&A[113]);	// latency = 1
  __mv_vop124 = _mm_i64gather_pd(&x[49], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm37 = _mm_fmadd_pd(__mv_vop123, __mv_vop124, __mv_accm37);	// latency = 5
  // stmt vectorized: y[50] += A[113] * x[49]
__mv_hi = _mm_shuffle_pd(__mv_accm37, __mv_accm37, 0x1);
__mv_accm37 = _mm_add_pd(__mv_accm37, __mv_hi);
y[50] = y[50]+__mv_accm37[0];;
  // stmt vectorized: y[50] += A[114] * x[51];
  __mv_accm38 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop126 = _mm256_maskload_pd(&A[((i0 + 0) + 115)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop127 = _mm256_maskload_pd(&x[((i0 + 0) + 50)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm38 = _mm256_fmadd_pd(__mv_vop126, __mv_vop127, __mv_accm38);	// latency = 5
    // stmt vectorized: y[0 * i0 + 51] += A[1 * i0 + 115] * x[1 * i0 + 50];
  }
  __mv_vop129 = _mm_loadu_pd(&A[118]);	// latency = 1
  __mv_vop130 = _mm_i64gather_pd(&x[51], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm39 = _mm_fmadd_pd(__mv_vop129, __mv_vop130, __mv_accm39);	// latency = 5
  // stmt vectorized: y[52] += A[118] * x[51]
__mv_hi = _mm_shuffle_pd(__mv_accm39, __mv_accm39, 0x1);
__mv_accm39 = _mm_add_pd(__mv_accm39, __mv_hi);
y[52] = y[52]+__mv_accm39[0];;
  // stmt vectorized: y[52] += A[119] * x[53];
  __mv_accm40 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop132 = _mm256_maskload_pd(&A[((i0 + 0) + 120)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop133 = _mm256_maskload_pd(&x[((i0 + 0) + 52)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm40 = _mm256_fmadd_pd(__mv_vop132, __mv_vop133, __mv_accm40);	// latency = 5
    // stmt vectorized: y[0 * i0 + 53] += A[1 * i0 + 120] * x[1 * i0 + 52];
  }__mv_accm34 = _mm256_hadd_pd(__mv_accm34, __mv_accm36);	// latency = 6
__mv_accm34 = _mm256_add_pd(_mm256_blend_pd(__mv_accm34,__mv_accm38,0b1100), _mm256_permute2f128_pd(__mv_accm34,__mv_accm38,0x21));	// latency = 4
y[47] = y[47] + __mv_accm34[0];	// latency = 1
y[49] = y[49] + __mv_accm34[1];	// latency = 1
y[51] = y[51] + __mv_accm34[2];	// latency = 1
y[53] = y[53] + __mv_accm34[3];	// latency = 1

  __mv_vop135 = _mm_loadu_pd(&A[123]);	// latency = 1
  __mv_vop136 = _mm_i64gather_pd(&x[53], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm41 = _mm_fmadd_pd(__mv_vop135, __mv_vop136, __mv_accm41);	// latency = 5
  // stmt vectorized: y[54] += A[123] * x[53]
__mv_hi = _mm_shuffle_pd(__mv_accm41, __mv_accm41, 0x1);
__mv_accm41 = _mm_add_pd(__mv_accm41, __mv_hi);
y[54] = y[54]+__mv_accm41[0];;
  // stmt vectorized: y[54] += A[124] * x[55];
  __mv_accm42 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop138 = _mm256_maskload_pd(&A[((i0 + 0) + 125)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop139 = _mm256_maskload_pd(&x[((i0 + 0) + 54)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm42 = _mm256_fmadd_pd(__mv_vop138, __mv_vop139, __mv_accm42);	// latency = 5
    // stmt vectorized: y[0 * i0 + 55] += A[1 * i0 + 125] * x[1 * i0 + 54];
  }
  __mv_vop141 = _mm_loadu_pd(&A[128]);	// latency = 1
  __mv_vop142 = _mm_i64gather_pd(&x[55], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm43 = _mm_fmadd_pd(__mv_vop141, __mv_vop142, __mv_accm43);	// latency = 5
  // stmt vectorized: y[56] += A[128] * x[55]
__mv_hi = _mm_shuffle_pd(__mv_accm43, __mv_accm43, 0x1);
__mv_accm43 = _mm_add_pd(__mv_accm43, __mv_hi);
y[56] = y[56]+__mv_accm43[0];;
  // stmt vectorized: y[56] += A[129] * x[57];
  __mv_accm44 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop144 = _mm256_maskload_pd(&A[((i0 + 0) + 130)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop145 = _mm256_maskload_pd(&x[((i0 + 0) + 56)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm44 = _mm256_fmadd_pd(__mv_vop144, __mv_vop145, __mv_accm44);	// latency = 5
    // stmt vectorized: y[0 * i0 + 57] += A[1 * i0 + 130] * x[1 * i0 + 56];
  }
  __mv_vop147 = _mm_loadu_pd(&A[133]);	// latency = 1
  __mv_vop148 = _mm_i64gather_pd(&x[57], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm45 = _mm_fmadd_pd(__mv_vop147, __mv_vop148, __mv_accm45);	// latency = 5
  // stmt vectorized: y[58] += A[133] * x[57]
__mv_hi = _mm_shuffle_pd(__mv_accm45, __mv_accm45, 0x1);
__mv_accm45 = _mm_add_pd(__mv_accm45, __mv_hi);
y[58] = y[58]+__mv_accm45[0];;
  // stmt vectorized: y[58] += A[134] * x[59];
  __mv_accm46 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop150 = _mm256_maskload_pd(&A[((i0 + 0) + 135)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop151 = _mm256_maskload_pd(&x[((i0 + 0) + 58)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm46 = _mm256_fmadd_pd(__mv_vop150, __mv_vop151, __mv_accm46);	// latency = 5
    // stmt vectorized: y[0 * i0 + 59] += A[1 * i0 + 135] * x[1 * i0 + 58];
  }
  __mv_vop153 = _mm_loadu_pd(&A[138]);	// latency = 1
  __mv_vop154 = _mm_i64gather_pd(&x[59], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm47 = _mm_fmadd_pd(__mv_vop153, __mv_vop154, __mv_accm47);	// latency = 5
  // stmt vectorized: y[60] += A[138] * x[59]
__mv_hi = _mm_shuffle_pd(__mv_accm47, __mv_accm47, 0x1);
__mv_accm47 = _mm_add_pd(__mv_accm47, __mv_hi);
y[60] = y[60]+__mv_accm47[0];;
  // stmt vectorized: y[60] += A[139] * x[61];
  __mv_accm48 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop156 = _mm256_maskload_pd(&A[((i0 + 0) + 140)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop157 = _mm256_maskload_pd(&x[((i0 + 0) + 60)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm48 = _mm256_fmadd_pd(__mv_vop156, __mv_vop157, __mv_accm48);	// latency = 5
    // stmt vectorized: y[0 * i0 + 61] += A[1 * i0 + 140] * x[1 * i0 + 60];
  }__mv_accm42 = _mm256_hadd_pd(__mv_accm42, __mv_accm44);	// latency = 6
__mv_accm42 = _mm256_add_pd(_mm256_blend_pd(__mv_accm42,__mv_accm46,0b1100), _mm256_permute2f128_pd(__mv_accm42,__mv_accm46,0x21));	// latency = 4
y[55] = y[55] + __mv_accm42[0];	// latency = 1
y[57] = y[57] + __mv_accm42[1];	// latency = 1
y[59] = y[59] + __mv_accm42[2];	// latency = 1
y[61] = y[61] + __mv_accm42[3];	// latency = 1

  __mv_vop159 = _mm_loadu_pd(&A[143]);	// latency = 1
  __mv_vop160 = _mm_i64gather_pd(&x[61], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm49 = _mm_fmadd_pd(__mv_vop159, __mv_vop160, __mv_accm49);	// latency = 5
  // stmt vectorized: y[62] += A[143] * x[61]
__mv_hi = _mm_shuffle_pd(__mv_accm49, __mv_accm49, 0x1);
__mv_accm49 = _mm_add_pd(__mv_accm49, __mv_hi);
y[62] = y[62]+__mv_accm49[0];;
  // stmt vectorized: y[62] += A[144] * x[63];
  __mv_accm50 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop162 = _mm256_maskload_pd(&A[((i0 + 0) + 145)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop163 = _mm256_maskload_pd(&x[((i0 + 0) + 62)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm50 = _mm256_fmadd_pd(__mv_vop162, __mv_vop163, __mv_accm50);	// latency = 5
    // stmt vectorized: y[0 * i0 + 63] += A[1 * i0 + 145] * x[1 * i0 + 62];
  }
  __mv_vop165 = _mm_loadu_pd(&A[148]);	// latency = 1
  __mv_vop166 = _mm_i64gather_pd(&x[63], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm51 = _mm_fmadd_pd(__mv_vop165, __mv_vop166, __mv_accm51);	// latency = 5
  // stmt vectorized: y[64] += A[148] * x[63]
__mv_hi = _mm_shuffle_pd(__mv_accm51, __mv_accm51, 0x1);
__mv_accm51 = _mm_add_pd(__mv_accm51, __mv_hi);
y[64] = y[64]+__mv_accm51[0];;
  // stmt vectorized: y[64] += A[149] * x[65];
  __mv_accm52 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop168 = _mm256_maskload_pd(&A[((i0 + 0) + 150)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop169 = _mm256_maskload_pd(&x[((i0 + 0) + 64)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm52 = _mm256_fmadd_pd(__mv_vop168, __mv_vop169, __mv_accm52);	// latency = 5
    // stmt vectorized: y[0 * i0 + 65] += A[1 * i0 + 150] * x[1 * i0 + 64];
  }
  __mv_vop171 = _mm_loadu_pd(&A[153]);	// latency = 1
  __mv_vop172 = _mm_i64gather_pd(&x[65], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm53 = _mm_fmadd_pd(__mv_vop171, __mv_vop172, __mv_accm53);	// latency = 5
  // stmt vectorized: y[66] += A[153] * x[65]
__mv_hi = _mm_shuffle_pd(__mv_accm53, __mv_accm53, 0x1);
__mv_accm53 = _mm_add_pd(__mv_accm53, __mv_hi);
y[66] = y[66]+__mv_accm53[0];;
  // stmt vectorized: y[66] += A[154] * x[67];
  __mv_accm54 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop174 = _mm256_maskload_pd(&A[((i0 + 0) + 155)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop175 = _mm256_maskload_pd(&x[((i0 + 0) + 66)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm54 = _mm256_fmadd_pd(__mv_vop174, __mv_vop175, __mv_accm54);	// latency = 5
    // stmt vectorized: y[0 * i0 + 67] += A[1 * i0 + 155] * x[1 * i0 + 66];
  }
  __mv_vop177 = _mm_loadu_pd(&A[158]);	// latency = 1
  __mv_vop178 = _mm_i64gather_pd(&x[67], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm55 = _mm_fmadd_pd(__mv_vop177, __mv_vop178, __mv_accm55);	// latency = 5
  // stmt vectorized: y[68] += A[158] * x[67]
__mv_hi = _mm_shuffle_pd(__mv_accm55, __mv_accm55, 0x1);
__mv_accm55 = _mm_add_pd(__mv_accm55, __mv_hi);
y[68] = y[68]+__mv_accm55[0];;
  // stmt vectorized: y[68] += A[159] * x[69];
  __mv_accm56 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop180 = _mm256_maskload_pd(&A[((i0 + 0) + 160)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop181 = _mm256_maskload_pd(&x[((i0 + 0) + 68)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm56 = _mm256_fmadd_pd(__mv_vop180, __mv_vop181, __mv_accm56);	// latency = 5
    // stmt vectorized: y[0 * i0 + 69] += A[1 * i0 + 160] * x[1 * i0 + 68];
  }__mv_accm50 = _mm256_hadd_pd(__mv_accm50, __mv_accm52);	// latency = 6
__mv_accm50 = _mm256_add_pd(_mm256_blend_pd(__mv_accm50,__mv_accm54,0b1100), _mm256_permute2f128_pd(__mv_accm50,__mv_accm54,0x21));	// latency = 4
y[63] = y[63] + __mv_accm50[0];	// latency = 1
y[65] = y[65] + __mv_accm50[1];	// latency = 1
y[67] = y[67] + __mv_accm50[2];	// latency = 1
y[69] = y[69] + __mv_accm50[3];	// latency = 1

  __mv_vop183 = _mm_loadu_pd(&A[163]);	// latency = 1
  __mv_vop184 = _mm_i64gather_pd(&x[69], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm57 = _mm_fmadd_pd(__mv_vop183, __mv_vop184, __mv_accm57);	// latency = 5
  // stmt vectorized: y[70] += A[163] * x[69]
__mv_hi = _mm_shuffle_pd(__mv_accm57, __mv_accm57, 0x1);
__mv_accm57 = _mm_add_pd(__mv_accm57, __mv_hi);
y[70] = y[70]+__mv_accm57[0];;
  // stmt vectorized: y[70] += A[164] * x[71];
  __mv_accm58 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop186 = _mm256_maskload_pd(&A[((i0 + 0) + 165)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop187 = _mm256_maskload_pd(&x[((i0 + 0) + 70)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm58 = _mm256_fmadd_pd(__mv_vop186, __mv_vop187, __mv_accm58);	// latency = 5
    // stmt vectorized: y[0 * i0 + 71] += A[1 * i0 + 165] * x[1 * i0 + 70];
  }
  __mv_vop189 = _mm_loadu_pd(&A[168]);	// latency = 1
  __mv_vop190 = _mm_i64gather_pd(&x[71], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm59 = _mm_fmadd_pd(__mv_vop189, __mv_vop190, __mv_accm59);	// latency = 5
  // stmt vectorized: y[72] += A[168] * x[71]
__mv_hi = _mm_shuffle_pd(__mv_accm59, __mv_accm59, 0x1);
__mv_accm59 = _mm_add_pd(__mv_accm59, __mv_hi);
y[72] = y[72]+__mv_accm59[0];;
  // stmt vectorized: y[72] += A[169] * x[73];
  __mv_accm60 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop192 = _mm256_maskload_pd(&A[((i0 + 0) + 170)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop193 = _mm256_maskload_pd(&x[((i0 + 0) + 72)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm60 = _mm256_fmadd_pd(__mv_vop192, __mv_vop193, __mv_accm60);	// latency = 5
    // stmt vectorized: y[0 * i0 + 73] += A[1 * i0 + 170] * x[1 * i0 + 72];
  }
  __mv_vop195 = _mm_loadu_pd(&A[173]);	// latency = 1
  __mv_vop196 = _mm_i64gather_pd(&x[73], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm61 = _mm_fmadd_pd(__mv_vop195, __mv_vop196, __mv_accm61);	// latency = 5
  // stmt vectorized: y[74] += A[173] * x[73]
__mv_hi = _mm_shuffle_pd(__mv_accm61, __mv_accm61, 0x1);
__mv_accm61 = _mm_add_pd(__mv_accm61, __mv_hi);
y[74] = y[74]+__mv_accm61[0];;
  // stmt vectorized: y[74] += A[174] * x[75];
  __mv_accm62 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop198 = _mm256_maskload_pd(&A[((i0 + 0) + 175)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop199 = _mm256_maskload_pd(&x[((i0 + 0) + 74)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm62 = _mm256_fmadd_pd(__mv_vop198, __mv_vop199, __mv_accm62);	// latency = 5
    // stmt vectorized: y[0 * i0 + 75] += A[1 * i0 + 175] * x[1 * i0 + 74];
  }
  __mv_vop201 = _mm_loadu_pd(&A[178]);	// latency = 1
  __mv_vop202 = _mm_i64gather_pd(&x[75], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm63 = _mm_fmadd_pd(__mv_vop201, __mv_vop202, __mv_accm63);	// latency = 5
  // stmt vectorized: y[76] += A[178] * x[75]
__mv_hi = _mm_shuffle_pd(__mv_accm63, __mv_accm63, 0x1);
__mv_accm63 = _mm_add_pd(__mv_accm63, __mv_hi);
y[76] = y[76]+__mv_accm63[0];;
  // stmt vectorized: y[76] += A[179] * x[77];
  __mv_accm64 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop204 = _mm256_maskload_pd(&A[((i0 + 0) + 180)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop205 = _mm256_maskload_pd(&x[((i0 + 0) + 76)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm64 = _mm256_fmadd_pd(__mv_vop204, __mv_vop205, __mv_accm64);	// latency = 5
    // stmt vectorized: y[0 * i0 + 77] += A[1 * i0 + 180] * x[1 * i0 + 76];
  }__mv_accm58 = _mm256_hadd_pd(__mv_accm58, __mv_accm60);	// latency = 6
__mv_accm58 = _mm256_add_pd(_mm256_blend_pd(__mv_accm58,__mv_accm62,0b1100), _mm256_permute2f128_pd(__mv_accm58,__mv_accm62,0x21));	// latency = 4
y[71] = y[71] + __mv_accm58[0];	// latency = 1
y[73] = y[73] + __mv_accm58[1];	// latency = 1
y[75] = y[75] + __mv_accm58[2];	// latency = 1
y[77] = y[77] + __mv_accm58[3];	// latency = 1

  __mv_vop207 = _mm_loadu_pd(&A[183]);	// latency = 1
  __mv_vop208 = _mm_i64gather_pd(&x[77], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm65 = _mm_fmadd_pd(__mv_vop207, __mv_vop208, __mv_accm65);	// latency = 5
  // stmt vectorized: y[78] += A[183] * x[77]
__mv_hi = _mm_shuffle_pd(__mv_accm65, __mv_accm65, 0x1);
__mv_accm65 = _mm_add_pd(__mv_accm65, __mv_hi);
y[78] = y[78]+__mv_accm65[0];;
  // stmt vectorized: y[78] += A[184] * x[79];
  __mv_accm66 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop210 = _mm256_maskload_pd(&A[((i0 + 0) + 185)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop211 = _mm256_maskload_pd(&x[((i0 + 0) + 78)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm66 = _mm256_fmadd_pd(__mv_vop210, __mv_vop211, __mv_accm66);	// latency = 5
    // stmt vectorized: y[0 * i0 + 79] += A[1 * i0 + 185] * x[1 * i0 + 78];
  }
  __mv_vop213 = _mm_loadu_pd(&A[188]);	// latency = 1
  __mv_vop214 = _mm_i64gather_pd(&x[79], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm67 = _mm_fmadd_pd(__mv_vop213, __mv_vop214, __mv_accm67);	// latency = 5
  // stmt vectorized: y[80] += A[188] * x[79]
__mv_hi = _mm_shuffle_pd(__mv_accm67, __mv_accm67, 0x1);
__mv_accm67 = _mm_add_pd(__mv_accm67, __mv_hi);
y[80] = y[80]+__mv_accm67[0];;
  // stmt vectorized: y[80] += A[189] * x[81];
  __mv_accm68 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop216 = _mm256_maskload_pd(&A[((i0 + 0) + 190)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop217 = _mm256_maskload_pd(&x[((i0 + 0) + 80)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm68 = _mm256_fmadd_pd(__mv_vop216, __mv_vop217, __mv_accm68);	// latency = 5
    // stmt vectorized: y[0 * i0 + 81] += A[1 * i0 + 190] * x[1 * i0 + 80];
  }
  __mv_vop219 = _mm_loadu_pd(&A[193]);	// latency = 1
  __mv_vop220 = _mm_i64gather_pd(&x[81], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm69 = _mm_fmadd_pd(__mv_vop219, __mv_vop220, __mv_accm69);	// latency = 5
  // stmt vectorized: y[82] += A[193] * x[81]
__mv_hi = _mm_shuffle_pd(__mv_accm69, __mv_accm69, 0x1);
__mv_accm69 = _mm_add_pd(__mv_accm69, __mv_hi);
y[82] = y[82]+__mv_accm69[0];;
  // stmt vectorized: y[82] += A[194] * x[83];
  __mv_accm70 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop222 = _mm256_maskload_pd(&A[((i0 + 0) + 195)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop223 = _mm256_maskload_pd(&x[((i0 + 0) + 82)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm70 = _mm256_fmadd_pd(__mv_vop222, __mv_vop223, __mv_accm70);	// latency = 5
    // stmt vectorized: y[0 * i0 + 83] += A[1 * i0 + 195] * x[1 * i0 + 82];
  }
  __mv_vop225 = _mm_loadu_pd(&A[198]);	// latency = 1
  __mv_vop226 = _mm_i64gather_pd(&x[83], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm71 = _mm_fmadd_pd(__mv_vop225, __mv_vop226, __mv_accm71);	// latency = 5
  // stmt vectorized: y[84] += A[198] * x[83]
__mv_hi = _mm_shuffle_pd(__mv_accm71, __mv_accm71, 0x1);
__mv_accm71 = _mm_add_pd(__mv_accm71, __mv_hi);
y[84] = y[84]+__mv_accm71[0];;
  // stmt vectorized: y[84] += A[199] * x[85];
  __mv_accm72 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop228 = _mm256_maskload_pd(&A[((i0 + 0) + 200)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop229 = _mm256_maskload_pd(&x[((i0 + 0) + 84)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm72 = _mm256_fmadd_pd(__mv_vop228, __mv_vop229, __mv_accm72);	// latency = 5
    // stmt vectorized: y[0 * i0 + 85] += A[1 * i0 + 200] * x[1 * i0 + 84];
  }__mv_accm66 = _mm256_hadd_pd(__mv_accm66, __mv_accm68);	// latency = 6
__mv_accm66 = _mm256_add_pd(_mm256_blend_pd(__mv_accm66,__mv_accm70,0b1100), _mm256_permute2f128_pd(__mv_accm66,__mv_accm70,0x21));	// latency = 4
y[79] = y[79] + __mv_accm66[0];	// latency = 1
y[81] = y[81] + __mv_accm66[1];	// latency = 1
y[83] = y[83] + __mv_accm66[2];	// latency = 1
y[85] = y[85] + __mv_accm66[3];	// latency = 1

  __mv_vop231 = _mm_loadu_pd(&A[203]);	// latency = 1
  __mv_vop232 = _mm_i64gather_pd(&x[85], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm73 = _mm_fmadd_pd(__mv_vop231, __mv_vop232, __mv_accm73);	// latency = 5
  // stmt vectorized: y[86] += A[203] * x[85]
__mv_hi = _mm_shuffle_pd(__mv_accm73, __mv_accm73, 0x1);
__mv_accm73 = _mm_add_pd(__mv_accm73, __mv_hi);
y[86] = y[86]+__mv_accm73[0];;
  // stmt vectorized: y[86] += A[204] * x[87];
  __mv_accm74 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop234 = _mm256_maskload_pd(&A[((i0 + 0) + 205)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop235 = _mm256_maskload_pd(&x[((i0 + 0) + 86)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm74 = _mm256_fmadd_pd(__mv_vop234, __mv_vop235, __mv_accm74);	// latency = 5
    // stmt vectorized: y[0 * i0 + 87] += A[1 * i0 + 205] * x[1 * i0 + 86];
  }
  __mv_vop237 = _mm_loadu_pd(&A[208]);	// latency = 1
  __mv_vop238 = _mm_i64gather_pd(&x[87], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm75 = _mm_fmadd_pd(__mv_vop237, __mv_vop238, __mv_accm75);	// latency = 5
  // stmt vectorized: y[88] += A[208] * x[87]
__mv_hi = _mm_shuffle_pd(__mv_accm75, __mv_accm75, 0x1);
__mv_accm75 = _mm_add_pd(__mv_accm75, __mv_hi);
y[88] = y[88]+__mv_accm75[0];;
  // stmt vectorized: y[88] += A[209] * x[89];
  __mv_accm76 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop240 = _mm256_maskload_pd(&A[((i0 + 0) + 210)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop241 = _mm256_maskload_pd(&x[((i0 + 0) + 88)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm76 = _mm256_fmadd_pd(__mv_vop240, __mv_vop241, __mv_accm76);	// latency = 5
    // stmt vectorized: y[0 * i0 + 89] += A[1 * i0 + 210] * x[1 * i0 + 88];
  }
  __mv_vop243 = _mm_loadu_pd(&A[213]);	// latency = 1
  __mv_vop244 = _mm_i64gather_pd(&x[89], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm77 = _mm_fmadd_pd(__mv_vop243, __mv_vop244, __mv_accm77);	// latency = 5
  // stmt vectorized: y[90] += A[213] * x[89]
__mv_hi = _mm_shuffle_pd(__mv_accm77, __mv_accm77, 0x1);
__mv_accm77 = _mm_add_pd(__mv_accm77, __mv_hi);
y[90] = y[90]+__mv_accm77[0];;
  // stmt vectorized: y[90] += A[214] * x[91];
  __mv_accm78 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop246 = _mm256_maskload_pd(&A[((i0 + 0) + 215)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop247 = _mm256_maskload_pd(&x[((i0 + 0) + 90)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm78 = _mm256_fmadd_pd(__mv_vop246, __mv_vop247, __mv_accm78);	// latency = 5
    // stmt vectorized: y[0 * i0 + 91] += A[1 * i0 + 215] * x[1 * i0 + 90];
  }
  __mv_vop249 = _mm_loadu_pd(&A[218]);	// latency = 1
  __mv_vop250 = _mm_i64gather_pd(&x[91], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm79 = _mm_fmadd_pd(__mv_vop249, __mv_vop250, __mv_accm79);	// latency = 5
  // stmt vectorized: y[92] += A[218] * x[91]
__mv_hi = _mm_shuffle_pd(__mv_accm79, __mv_accm79, 0x1);
__mv_accm79 = _mm_add_pd(__mv_accm79, __mv_hi);
y[92] = y[92]+__mv_accm79[0];;
  // stmt vectorized: y[92] += A[219] * x[93];
  __mv_accm80 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop252 = _mm256_maskload_pd(&A[((i0 + 0) + 220)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop253 = _mm256_maskload_pd(&x[((i0 + 0) + 92)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm80 = _mm256_fmadd_pd(__mv_vop252, __mv_vop253, __mv_accm80);	// latency = 5
    // stmt vectorized: y[0 * i0 + 93] += A[1 * i0 + 220] * x[1 * i0 + 92];
  }__mv_accm74 = _mm256_hadd_pd(__mv_accm74, __mv_accm76);	// latency = 6
__mv_accm74 = _mm256_add_pd(_mm256_blend_pd(__mv_accm74,__mv_accm78,0b1100), _mm256_permute2f128_pd(__mv_accm74,__mv_accm78,0x21));	// latency = 4
y[87] = y[87] + __mv_accm74[0];	// latency = 1
y[89] = y[89] + __mv_accm74[1];	// latency = 1
y[91] = y[91] + __mv_accm74[2];	// latency = 1
y[93] = y[93] + __mv_accm74[3];	// latency = 1

  __mv_vop255 = _mm_loadu_pd(&A[223]);	// latency = 1
  __mv_vop256 = _mm_i64gather_pd(&x[93], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm81 = _mm_fmadd_pd(__mv_vop255, __mv_vop256, __mv_accm81);	// latency = 5
  // stmt vectorized: y[94] += A[223] * x[93]
__mv_hi = _mm_shuffle_pd(__mv_accm81, __mv_accm81, 0x1);
__mv_accm81 = _mm_add_pd(__mv_accm81, __mv_hi);
y[94] = y[94]+__mv_accm81[0];;
  // stmt vectorized: y[94] += A[224] * x[95];
  __mv_accm82 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop258 = _mm256_maskload_pd(&A[((i0 + 0) + 225)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop259 = _mm256_maskload_pd(&x[((i0 + 0) + 94)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm82 = _mm256_fmadd_pd(__mv_vop258, __mv_vop259, __mv_accm82);	// latency = 5
    // stmt vectorized: y[0 * i0 + 95] += A[1 * i0 + 225] * x[1 * i0 + 94];
  }
  __mv_vop261 = _mm_loadu_pd(&A[228]);	// latency = 1
  __mv_vop262 = _mm_i64gather_pd(&x[95], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm83 = _mm_fmadd_pd(__mv_vop261, __mv_vop262, __mv_accm83);	// latency = 5
  // stmt vectorized: y[96] += A[228] * x[95]
__mv_hi = _mm_shuffle_pd(__mv_accm83, __mv_accm83, 0x1);
__mv_accm83 = _mm_add_pd(__mv_accm83, __mv_hi);
y[96] = y[96]+__mv_accm83[0];;
  // stmt vectorized: y[96] += A[229] * x[97];
  __mv_accm84 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop264 = _mm256_maskload_pd(&A[((i0 + 0) + 230)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop265 = _mm256_maskload_pd(&x[((i0 + 0) + 96)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm84 = _mm256_fmadd_pd(__mv_vop264, __mv_vop265, __mv_accm84);	// latency = 5
    // stmt vectorized: y[0 * i0 + 97] += A[1 * i0 + 230] * x[1 * i0 + 96];
  }
  __mv_vop267 = _mm_loadu_pd(&A[233]);	// latency = 1
  __mv_vop268 = _mm_i64gather_pd(&x[97], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm85 = _mm_fmadd_pd(__mv_vop267, __mv_vop268, __mv_accm85);	// latency = 5
  // stmt vectorized: y[98] += A[233] * x[97]
__mv_hi = _mm_shuffle_pd(__mv_accm85, __mv_accm85, 0x1);
__mv_accm85 = _mm_add_pd(__mv_accm85, __mv_hi);
y[98] = y[98]+__mv_accm85[0];;
  // stmt vectorized: y[98] += A[234] * x[99];
  __mv_accm86 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop270 = _mm256_maskload_pd(&A[((i0 + 0) + 235)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop271 = _mm256_maskload_pd(&x[((i0 + 0) + 98)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm86 = _mm256_fmadd_pd(__mv_vop270, __mv_vop271, __mv_accm86);	// latency = 5
    // stmt vectorized: y[0 * i0 + 99] += A[1 * i0 + 235] * x[1 * i0 + 98];
  }
  __mv_vop273 = _mm_loadu_pd(&A[238]);	// latency = 1
  __mv_vop274 = _mm_i64gather_pd(&x[99], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm87 = _mm_fmadd_pd(__mv_vop273, __mv_vop274, __mv_accm87);	// latency = 5
  // stmt vectorized: y[100] += A[238] * x[99]
__mv_hi = _mm_shuffle_pd(__mv_accm87, __mv_accm87, 0x1);
__mv_accm87 = _mm_add_pd(__mv_accm87, __mv_hi);
y[100] = y[100]+__mv_accm87[0];;
  // stmt vectorized: y[100] += A[239] * x[101];
  __mv_accm88 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop276 = _mm256_maskload_pd(&A[((i0 + 0) + 240)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop277 = _mm256_maskload_pd(&x[((i0 + 0) + 100)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm88 = _mm256_fmadd_pd(__mv_vop276, __mv_vop277, __mv_accm88);	// latency = 5
    // stmt vectorized: y[0 * i0 + 101] += A[1 * i0 + 240] * x[1 * i0 + 100];
  }__mv_accm82 = _mm256_hadd_pd(__mv_accm82, __mv_accm84);	// latency = 6
__mv_accm82 = _mm256_add_pd(_mm256_blend_pd(__mv_accm82,__mv_accm86,0b1100), _mm256_permute2f128_pd(__mv_accm82,__mv_accm86,0x21));	// latency = 4
y[95] = y[95] + __mv_accm82[0];	// latency = 1
y[97] = y[97] + __mv_accm82[1];	// latency = 1
y[99] = y[99] + __mv_accm82[2];	// latency = 1
y[101] = y[101] + __mv_accm82[3];	// latency = 1

  __mv_vop279 = _mm_loadu_pd(&A[243]);	// latency = 1
  __mv_vop280 = _mm_i64gather_pd(&x[101], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm89 = _mm_fmadd_pd(__mv_vop279, __mv_vop280, __mv_accm89);	// latency = 5
  // stmt vectorized: y[102] += A[243] * x[101]
__mv_hi = _mm_shuffle_pd(__mv_accm89, __mv_accm89, 0x1);
__mv_accm89 = _mm_add_pd(__mv_accm89, __mv_hi);
y[102] = y[102]+__mv_accm89[0];;
  // stmt vectorized: y[102] += A[244] * x[103];
  __mv_accm90 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop282 = _mm256_maskload_pd(&A[((i0 + 0) + 245)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop283 = _mm256_maskload_pd(&x[((i0 + 0) + 102)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm90 = _mm256_fmadd_pd(__mv_vop282, __mv_vop283, __mv_accm90);	// latency = 5
    // stmt vectorized: y[0 * i0 + 103] += A[1 * i0 + 245] * x[1 * i0 + 102];
  }
  __mv_vop285 = _mm_loadu_pd(&A[248]);	// latency = 1
  __mv_vop286 = _mm_i64gather_pd(&x[103], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm91 = _mm_fmadd_pd(__mv_vop285, __mv_vop286, __mv_accm91);	// latency = 5
  // stmt vectorized: y[104] += A[248] * x[103]
__mv_hi = _mm_shuffle_pd(__mv_accm91, __mv_accm91, 0x1);
__mv_accm91 = _mm_add_pd(__mv_accm91, __mv_hi);
y[104] = y[104]+__mv_accm91[0];;
  // stmt vectorized: y[104] += A[249] * x[105];
  __mv_accm92 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop288 = _mm256_maskload_pd(&A[((i0 + 0) + 250)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop289 = _mm256_maskload_pd(&x[((i0 + 0) + 104)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm92 = _mm256_fmadd_pd(__mv_vop288, __mv_vop289, __mv_accm92);	// latency = 5
    // stmt vectorized: y[0 * i0 + 105] += A[1 * i0 + 250] * x[1 * i0 + 104];
  }
  __mv_vop291 = _mm_loadu_pd(&A[253]);	// latency = 1
  __mv_vop292 = _mm_i64gather_pd(&x[105], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm93 = _mm_fmadd_pd(__mv_vop291, __mv_vop292, __mv_accm93);	// latency = 5
  // stmt vectorized: y[106] += A[253] * x[105]
__mv_hi = _mm_shuffle_pd(__mv_accm93, __mv_accm93, 0x1);
__mv_accm93 = _mm_add_pd(__mv_accm93, __mv_hi);
y[106] = y[106]+__mv_accm93[0];;
  // stmt vectorized: y[106] += A[254] * x[107];
  __mv_accm94 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop294 = _mm256_maskload_pd(&A[((i0 + 0) + 255)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop295 = _mm256_maskload_pd(&x[((i0 + 0) + 106)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm94 = _mm256_fmadd_pd(__mv_vop294, __mv_vop295, __mv_accm94);	// latency = 5
    // stmt vectorized: y[0 * i0 + 107] += A[1 * i0 + 255] * x[1 * i0 + 106];
  }
  __mv_vop297 = _mm_loadu_pd(&A[258]);	// latency = 1
  __mv_vop298 = _mm_i64gather_pd(&x[107], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm95 = _mm_fmadd_pd(__mv_vop297, __mv_vop298, __mv_accm95);	// latency = 5
  // stmt vectorized: y[108] += A[258] * x[107]
__mv_hi = _mm_shuffle_pd(__mv_accm95, __mv_accm95, 0x1);
__mv_accm95 = _mm_add_pd(__mv_accm95, __mv_hi);
y[108] = y[108]+__mv_accm95[0];;
  // stmt vectorized: y[108] += A[259] * x[109];
  __mv_accm96 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop300 = _mm256_maskload_pd(&A[((i0 + 0) + 260)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop301 = _mm256_maskload_pd(&x[((i0 + 0) + 108)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm96 = _mm256_fmadd_pd(__mv_vop300, __mv_vop301, __mv_accm96);	// latency = 5
    // stmt vectorized: y[0 * i0 + 109] += A[1 * i0 + 260] * x[1 * i0 + 108];
  }__mv_accm90 = _mm256_hadd_pd(__mv_accm90, __mv_accm92);	// latency = 6
__mv_accm90 = _mm256_add_pd(_mm256_blend_pd(__mv_accm90,__mv_accm94,0b1100), _mm256_permute2f128_pd(__mv_accm90,__mv_accm94,0x21));	// latency = 4
y[103] = y[103] + __mv_accm90[0];	// latency = 1
y[105] = y[105] + __mv_accm90[1];	// latency = 1
y[107] = y[107] + __mv_accm90[2];	// latency = 1
y[109] = y[109] + __mv_accm90[3];	// latency = 1

  __mv_vop303 = _mm_loadu_pd(&A[263]);	// latency = 1
  __mv_vop304 = _mm_i64gather_pd(&x[109], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm97 = _mm_fmadd_pd(__mv_vop303, __mv_vop304, __mv_accm97);	// latency = 5
  // stmt vectorized: y[110] += A[263] * x[109]
__mv_hi = _mm_shuffle_pd(__mv_accm97, __mv_accm97, 0x1);
__mv_accm97 = _mm_add_pd(__mv_accm97, __mv_hi);
y[110] = y[110]+__mv_accm97[0];;
  // stmt vectorized: y[110] += A[264] * x[111];
  __mv_accm98 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop306 = _mm256_maskload_pd(&A[((i0 + 0) + 265)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop307 = _mm256_maskload_pd(&x[((i0 + 0) + 110)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm98 = _mm256_fmadd_pd(__mv_vop306, __mv_vop307, __mv_accm98);	// latency = 5
    // stmt vectorized: y[0 * i0 + 111] += A[1 * i0 + 265] * x[1 * i0 + 110];
  }
  __mv_vop309 = _mm_loadu_pd(&A[268]);	// latency = 1
  __mv_vop310 = _mm_i64gather_pd(&x[111], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm99 = _mm_fmadd_pd(__mv_vop309, __mv_vop310, __mv_accm99);	// latency = 5
  // stmt vectorized: y[112] += A[268] * x[111]
__mv_hi = _mm_shuffle_pd(__mv_accm99, __mv_accm99, 0x1);
__mv_accm99 = _mm_add_pd(__mv_accm99, __mv_hi);
y[112] = y[112]+__mv_accm99[0];;
  // stmt vectorized: y[112] += A[269] * x[113];
  __mv_accm100 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop312 = _mm256_maskload_pd(&A[((i0 + 0) + 270)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop313 = _mm256_maskload_pd(&x[((i0 + 0) + 112)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm100 = _mm256_fmadd_pd(__mv_vop312, __mv_vop313, __mv_accm100);	// latency = 5
    // stmt vectorized: y[0 * i0 + 113] += A[1 * i0 + 270] * x[1 * i0 + 112];
  }
  __mv_vop315 = _mm_loadu_pd(&A[273]);	// latency = 1
  __mv_vop316 = _mm_i64gather_pd(&x[113], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm101 = _mm_fmadd_pd(__mv_vop315, __mv_vop316, __mv_accm101);	// latency = 5
  // stmt vectorized: y[114] += A[273] * x[113]
__mv_hi = _mm_shuffle_pd(__mv_accm101, __mv_accm101, 0x1);
__mv_accm101 = _mm_add_pd(__mv_accm101, __mv_hi);
y[114] = y[114]+__mv_accm101[0];;
  // stmt vectorized: y[114] += A[274] * x[115];
  __mv_accm102 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop318 = _mm256_maskload_pd(&A[((i0 + 0) + 275)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop319 = _mm256_maskload_pd(&x[((i0 + 0) + 114)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm102 = _mm256_fmadd_pd(__mv_vop318, __mv_vop319, __mv_accm102);	// latency = 5
    // stmt vectorized: y[0 * i0 + 115] += A[1 * i0 + 275] * x[1 * i0 + 114];
  }
  __mv_vop321 = _mm_loadu_pd(&A[278]);	// latency = 1
  __mv_vop322 = _mm_i64gather_pd(&x[115], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm103 = _mm_fmadd_pd(__mv_vop321, __mv_vop322, __mv_accm103);	// latency = 5
  // stmt vectorized: y[116] += A[278] * x[115]
__mv_hi = _mm_shuffle_pd(__mv_accm103, __mv_accm103, 0x1);
__mv_accm103 = _mm_add_pd(__mv_accm103, __mv_hi);
y[116] = y[116]+__mv_accm103[0];;
  // stmt vectorized: y[116] += A[279] * x[117];
  __mv_accm104 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop324 = _mm256_maskload_pd(&A[((i0 + 0) + 280)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop325 = _mm256_maskload_pd(&x[((i0 + 0) + 116)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm104 = _mm256_fmadd_pd(__mv_vop324, __mv_vop325, __mv_accm104);	// latency = 5
    // stmt vectorized: y[0 * i0 + 117] += A[1 * i0 + 280] * x[1 * i0 + 116];
  }__mv_accm98 = _mm256_hadd_pd(__mv_accm98, __mv_accm100);	// latency = 6
__mv_accm98 = _mm256_add_pd(_mm256_blend_pd(__mv_accm98,__mv_accm102,0b1100), _mm256_permute2f128_pd(__mv_accm98,__mv_accm102,0x21));	// latency = 4
y[111] = y[111] + __mv_accm98[0];	// latency = 1
y[113] = y[113] + __mv_accm98[1];	// latency = 1
y[115] = y[115] + __mv_accm98[2];	// latency = 1
y[117] = y[117] + __mv_accm98[3];	// latency = 1

  __mv_vop327 = _mm_loadu_pd(&A[283]);	// latency = 1
  __mv_vop328 = _mm_i64gather_pd(&x[117], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm105 = _mm_fmadd_pd(__mv_vop327, __mv_vop328, __mv_accm105);	// latency = 5
  // stmt vectorized: y[118] += A[283] * x[117]
__mv_hi = _mm_shuffle_pd(__mv_accm105, __mv_accm105, 0x1);
__mv_accm105 = _mm_add_pd(__mv_accm105, __mv_hi);
y[118] = y[118]+__mv_accm105[0];;
  // stmt vectorized: y[118] += A[284] * x[119];
  __mv_accm106 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop330 = _mm256_maskload_pd(&A[((i0 + 0) + 285)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop331 = _mm256_maskload_pd(&x[((i0 + 0) + 118)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm106 = _mm256_fmadd_pd(__mv_vop330, __mv_vop331, __mv_accm106);	// latency = 5
    // stmt vectorized: y[0 * i0 + 119] += A[1 * i0 + 285] * x[1 * i0 + 118];
  }
  __mv_vop333 = _mm_loadu_pd(&A[288]);	// latency = 1
  __mv_vop334 = _mm_i64gather_pd(&x[119], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm107 = _mm_fmadd_pd(__mv_vop333, __mv_vop334, __mv_accm107);	// latency = 5
  // stmt vectorized: y[120] += A[288] * x[119]
__mv_hi = _mm_shuffle_pd(__mv_accm107, __mv_accm107, 0x1);
__mv_accm107 = _mm_add_pd(__mv_accm107, __mv_hi);
y[120] = y[120]+__mv_accm107[0];;
  // stmt vectorized: y[120] += A[289] * x[121];
  __mv_accm108 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop336 = _mm256_maskload_pd(&A[((i0 + 0) + 290)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop337 = _mm256_maskload_pd(&x[((i0 + 0) + 120)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm108 = _mm256_fmadd_pd(__mv_vop336, __mv_vop337, __mv_accm108);	// latency = 5
    // stmt vectorized: y[0 * i0 + 121] += A[1 * i0 + 290] * x[1 * i0 + 120];
  }
  __mv_vop339 = _mm_loadu_pd(&A[293]);	// latency = 1
  __mv_vop340 = _mm_i64gather_pd(&x[121], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm109 = _mm_fmadd_pd(__mv_vop339, __mv_vop340, __mv_accm109);	// latency = 5
  // stmt vectorized: y[122] += A[293] * x[121]
__mv_hi = _mm_shuffle_pd(__mv_accm109, __mv_accm109, 0x1);
__mv_accm109 = _mm_add_pd(__mv_accm109, __mv_hi);
y[122] = y[122]+__mv_accm109[0];;
  // stmt vectorized: y[122] += A[294] * x[123];
  __mv_accm110 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop342 = _mm256_maskload_pd(&A[((i0 + 0) + 295)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop343 = _mm256_maskload_pd(&x[((i0 + 0) + 122)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm110 = _mm256_fmadd_pd(__mv_vop342, __mv_vop343, __mv_accm110);	// latency = 5
    // stmt vectorized: y[0 * i0 + 123] += A[1 * i0 + 295] * x[1 * i0 + 122];
  }
  __mv_vop345 = _mm_loadu_pd(&A[298]);	// latency = 1
  __mv_vop346 = _mm_i64gather_pd(&x[123], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm111 = _mm_fmadd_pd(__mv_vop345, __mv_vop346, __mv_accm111);	// latency = 5
  // stmt vectorized: y[124] += A[298] * x[123]
__mv_hi = _mm_shuffle_pd(__mv_accm111, __mv_accm111, 0x1);
__mv_accm111 = _mm_add_pd(__mv_accm111, __mv_hi);
y[124] = y[124]+__mv_accm111[0];;
  // stmt vectorized: y[124] += A[299] * x[125];
  __mv_accm112 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop348 = _mm256_maskload_pd(&A[((i0 + 0) + 300)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop349 = _mm256_maskload_pd(&x[((i0 + 0) + 124)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm112 = _mm256_fmadd_pd(__mv_vop348, __mv_vop349, __mv_accm112);	// latency = 5
    // stmt vectorized: y[0 * i0 + 125] += A[1 * i0 + 300] * x[1 * i0 + 124];
  }__mv_accm106 = _mm256_hadd_pd(__mv_accm106, __mv_accm108);	// latency = 6
__mv_accm106 = _mm256_add_pd(_mm256_blend_pd(__mv_accm106,__mv_accm110,0b1100), _mm256_permute2f128_pd(__mv_accm106,__mv_accm110,0x21));	// latency = 4
y[119] = y[119] + __mv_accm106[0];	// latency = 1
y[121] = y[121] + __mv_accm106[1];	// latency = 1
y[123] = y[123] + __mv_accm106[2];	// latency = 1
y[125] = y[125] + __mv_accm106[3];	// latency = 1

  __mv_vop351 = _mm_loadu_pd(&A[303]);	// latency = 1
  __mv_vop352 = _mm_i64gather_pd(&x[125], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm113 = _mm_fmadd_pd(__mv_vop351, __mv_vop352, __mv_accm113);	// latency = 5
  // stmt vectorized: y[126] += A[303] * x[125]
__mv_hi = _mm_shuffle_pd(__mv_accm113, __mv_accm113, 0x1);
__mv_accm113 = _mm_add_pd(__mv_accm113, __mv_hi);
y[126] = y[126]+__mv_accm113[0];;
  // stmt vectorized: y[126] += A[304] * x[127];
  __mv_accm114 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop354 = _mm256_maskload_pd(&A[((i0 + 0) + 305)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop355 = _mm256_maskload_pd(&x[((i0 + 0) + 126)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm114 = _mm256_fmadd_pd(__mv_vop354, __mv_vop355, __mv_accm114);	// latency = 5
    // stmt vectorized: y[0 * i0 + 127] += A[1 * i0 + 305] * x[1 * i0 + 126];
  }
  __mv_vop357 = _mm_loadu_pd(&A[308]);	// latency = 1
  __mv_vop358 = _mm_i64gather_pd(&x[127], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm115 = _mm_fmadd_pd(__mv_vop357, __mv_vop358, __mv_accm115);	// latency = 5
  // stmt vectorized: y[128] += A[308] * x[127]
__mv_hi = _mm_shuffle_pd(__mv_accm115, __mv_accm115, 0x1);
__mv_accm115 = _mm_add_pd(__mv_accm115, __mv_hi);
y[128] = y[128]+__mv_accm115[0];;
  // stmt vectorized: y[128] += A[309] * x[129];
  __mv_accm116 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop360 = _mm256_maskload_pd(&A[((i0 + 0) + 310)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop361 = _mm256_maskload_pd(&x[((i0 + 0) + 128)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm116 = _mm256_fmadd_pd(__mv_vop360, __mv_vop361, __mv_accm116);	// latency = 5
    // stmt vectorized: y[0 * i0 + 129] += A[1 * i0 + 310] * x[1 * i0 + 128];
  }
  __mv_vop363 = _mm_loadu_pd(&A[313]);	// latency = 1
  __mv_vop364 = _mm_i64gather_pd(&x[129], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm117 = _mm_fmadd_pd(__mv_vop363, __mv_vop364, __mv_accm117);	// latency = 5
  // stmt vectorized: y[130] += A[313] * x[129]
__mv_hi = _mm_shuffle_pd(__mv_accm117, __mv_accm117, 0x1);
__mv_accm117 = _mm_add_pd(__mv_accm117, __mv_hi);
y[130] = y[130]+__mv_accm117[0];;
  // stmt vectorized: y[130] += A[314] * x[131];
  __mv_accm118 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop366 = _mm256_maskload_pd(&A[((i0 + 0) + 315)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop367 = _mm256_maskload_pd(&x[((i0 + 0) + 130)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm118 = _mm256_fmadd_pd(__mv_vop366, __mv_vop367, __mv_accm118);	// latency = 5
    // stmt vectorized: y[0 * i0 + 131] += A[1 * i0 + 315] * x[1 * i0 + 130];
  }
  __mv_vop369 = _mm_loadu_pd(&A[318]);	// latency = 1
  __mv_vop370 = _mm_i64gather_pd(&x[131], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm119 = _mm_fmadd_pd(__mv_vop369, __mv_vop370, __mv_accm119);	// latency = 5
  // stmt vectorized: y[132] += A[318] * x[131]
__mv_hi = _mm_shuffle_pd(__mv_accm119, __mv_accm119, 0x1);
__mv_accm119 = _mm_add_pd(__mv_accm119, __mv_hi);
y[132] = y[132]+__mv_accm119[0];;
  // stmt vectorized: y[132] += A[319] * x[133];
  __mv_accm120 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop372 = _mm256_maskload_pd(&A[((i0 + 0) + 320)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop373 = _mm256_maskload_pd(&x[((i0 + 0) + 132)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm120 = _mm256_fmadd_pd(__mv_vop372, __mv_vop373, __mv_accm120);	// latency = 5
    // stmt vectorized: y[0 * i0 + 133] += A[1 * i0 + 320] * x[1 * i0 + 132];
  }__mv_accm114 = _mm256_hadd_pd(__mv_accm114, __mv_accm116);	// latency = 6
__mv_accm114 = _mm256_add_pd(_mm256_blend_pd(__mv_accm114,__mv_accm118,0b1100), _mm256_permute2f128_pd(__mv_accm114,__mv_accm118,0x21));	// latency = 4
y[127] = y[127] + __mv_accm114[0];	// latency = 1
y[129] = y[129] + __mv_accm114[1];	// latency = 1
y[131] = y[131] + __mv_accm114[2];	// latency = 1
y[133] = y[133] + __mv_accm114[3];	// latency = 1

  __mv_vop375 = _mm_loadu_pd(&A[323]);	// latency = 1
  __mv_vop376 = _mm_i64gather_pd(&x[133], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm121 = _mm_fmadd_pd(__mv_vop375, __mv_vop376, __mv_accm121);	// latency = 5
  // stmt vectorized: y[134] += A[323] * x[133]
__mv_hi = _mm_shuffle_pd(__mv_accm121, __mv_accm121, 0x1);
__mv_accm121 = _mm_add_pd(__mv_accm121, __mv_hi);
y[134] = y[134]+__mv_accm121[0];;
  // stmt vectorized: y[134] += A[324] * x[135];
  __mv_accm122 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop378 = _mm256_maskload_pd(&A[((i0 + 0) + 325)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop379 = _mm256_maskload_pd(&x[((i0 + 0) + 134)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm122 = _mm256_fmadd_pd(__mv_vop378, __mv_vop379, __mv_accm122);	// latency = 5
    // stmt vectorized: y[0 * i0 + 135] += A[1 * i0 + 325] * x[1 * i0 + 134];
  }
  __mv_vop381 = _mm_loadu_pd(&A[328]);	// latency = 1
  __mv_vop382 = _mm_i64gather_pd(&x[135], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm123 = _mm_fmadd_pd(__mv_vop381, __mv_vop382, __mv_accm123);	// latency = 5
  // stmt vectorized: y[136] += A[328] * x[135]
__mv_hi = _mm_shuffle_pd(__mv_accm123, __mv_accm123, 0x1);
__mv_accm123 = _mm_add_pd(__mv_accm123, __mv_hi);
y[136] = y[136]+__mv_accm123[0];;
  // stmt vectorized: y[136] += A[329] * x[137];
  __mv_accm124 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop384 = _mm256_maskload_pd(&A[((i0 + 0) + 330)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop385 = _mm256_maskload_pd(&x[((i0 + 0) + 136)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm124 = _mm256_fmadd_pd(__mv_vop384, __mv_vop385, __mv_accm124);	// latency = 5
    // stmt vectorized: y[0 * i0 + 137] += A[1 * i0 + 330] * x[1 * i0 + 136];
  }
  __mv_vop387 = _mm_loadu_pd(&A[333]);	// latency = 1
  __mv_vop388 = _mm_i64gather_pd(&x[137], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm125 = _mm_fmadd_pd(__mv_vop387, __mv_vop388, __mv_accm125);	// latency = 5
  // stmt vectorized: y[138] += A[333] * x[137]
__mv_hi = _mm_shuffle_pd(__mv_accm125, __mv_accm125, 0x1);
__mv_accm125 = _mm_add_pd(__mv_accm125, __mv_hi);
y[138] = y[138]+__mv_accm125[0];;
  // stmt vectorized: y[138] += A[334] * x[139];
  __mv_accm126 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop390 = _mm256_maskload_pd(&A[((i0 + 0) + 335)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop391 = _mm256_maskload_pd(&x[((i0 + 0) + 138)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm126 = _mm256_fmadd_pd(__mv_vop390, __mv_vop391, __mv_accm126);	// latency = 5
    // stmt vectorized: y[0 * i0 + 139] += A[1 * i0 + 335] * x[1 * i0 + 138];
  }
  __mv_vop393 = _mm_loadu_pd(&A[338]);	// latency = 1
  __mv_vop394 = _mm_i64gather_pd(&x[139], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm127 = _mm_fmadd_pd(__mv_vop393, __mv_vop394, __mv_accm127);	// latency = 5
  // stmt vectorized: y[140] += A[338] * x[139]
__mv_hi = _mm_shuffle_pd(__mv_accm127, __mv_accm127, 0x1);
__mv_accm127 = _mm_add_pd(__mv_accm127, __mv_hi);
y[140] = y[140]+__mv_accm127[0];;
  // stmt vectorized: y[140] += A[339] * x[141];
  __mv_accm128 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop396 = _mm256_maskload_pd(&A[((i0 + 0) + 340)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop397 = _mm256_maskload_pd(&x[((i0 + 0) + 140)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm128 = _mm256_fmadd_pd(__mv_vop396, __mv_vop397, __mv_accm128);	// latency = 5
    // stmt vectorized: y[0 * i0 + 141] += A[1 * i0 + 340] * x[1 * i0 + 140];
  }__mv_accm122 = _mm256_hadd_pd(__mv_accm122, __mv_accm124);	// latency = 6
__mv_accm122 = _mm256_add_pd(_mm256_blend_pd(__mv_accm122,__mv_accm126,0b1100), _mm256_permute2f128_pd(__mv_accm122,__mv_accm126,0x21));	// latency = 4
y[135] = y[135] + __mv_accm122[0];	// latency = 1
y[137] = y[137] + __mv_accm122[1];	// latency = 1
y[139] = y[139] + __mv_accm122[2];	// latency = 1
y[141] = y[141] + __mv_accm122[3];	// latency = 1

  __mv_vop399 = _mm_loadu_pd(&A[343]);	// latency = 1
  __mv_vop400 = _mm_i64gather_pd(&x[141], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm129 = _mm_fmadd_pd(__mv_vop399, __mv_vop400, __mv_accm129);	// latency = 5
  // stmt vectorized: y[142] += A[343] * x[141]
__mv_hi = _mm_shuffle_pd(__mv_accm129, __mv_accm129, 0x1);
__mv_accm129 = _mm_add_pd(__mv_accm129, __mv_hi);
y[142] = y[142]+__mv_accm129[0];;
  // stmt vectorized: y[142] += A[344] * x[143];
  __mv_accm130 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop402 = _mm256_maskload_pd(&A[((i0 + 0) + 345)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop403 = _mm256_maskload_pd(&x[((i0 + 0) + 142)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm130 = _mm256_fmadd_pd(__mv_vop402, __mv_vop403, __mv_accm130);	// latency = 5
    // stmt vectorized: y[0 * i0 + 143] += A[1 * i0 + 345] * x[1 * i0 + 142];
  }
  __mv_vop405 = _mm_loadu_pd(&A[348]);	// latency = 1
  __mv_vop406 = _mm_i64gather_pd(&x[143], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm131 = _mm_fmadd_pd(__mv_vop405, __mv_vop406, __mv_accm131);	// latency = 5
  // stmt vectorized: y[144] += A[348] * x[143]
__mv_hi = _mm_shuffle_pd(__mv_accm131, __mv_accm131, 0x1);
__mv_accm131 = _mm_add_pd(__mv_accm131, __mv_hi);
y[144] = y[144]+__mv_accm131[0];;
  // stmt vectorized: y[144] += A[349] * x[145];
  __mv_accm132 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop408 = _mm256_maskload_pd(&A[((i0 + 0) + 350)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop409 = _mm256_maskload_pd(&x[((i0 + 0) + 144)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm132 = _mm256_fmadd_pd(__mv_vop408, __mv_vop409, __mv_accm132);	// latency = 5
    // stmt vectorized: y[0 * i0 + 145] += A[1 * i0 + 350] * x[1 * i0 + 144];
  }
  __mv_vop411 = _mm_loadu_pd(&A[353]);	// latency = 1
  __mv_vop412 = _mm_i64gather_pd(&x[145], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm133 = _mm_fmadd_pd(__mv_vop411, __mv_vop412, __mv_accm133);	// latency = 5
  // stmt vectorized: y[146] += A[353] * x[145]
__mv_hi = _mm_shuffle_pd(__mv_accm133, __mv_accm133, 0x1);
__mv_accm133 = _mm_add_pd(__mv_accm133, __mv_hi);
y[146] = y[146]+__mv_accm133[0];;
  // stmt vectorized: y[146] += A[354] * x[147];
  __mv_accm134 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop414 = _mm256_maskload_pd(&A[((i0 + 0) + 355)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop415 = _mm256_maskload_pd(&x[((i0 + 0) + 146)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm134 = _mm256_fmadd_pd(__mv_vop414, __mv_vop415, __mv_accm134);	// latency = 5
    // stmt vectorized: y[0 * i0 + 147] += A[1 * i0 + 355] * x[1 * i0 + 146];
  }
  __mv_vop417 = _mm_loadu_pd(&A[358]);	// latency = 1
  __mv_vop418 = _mm_i64gather_pd(&x[147], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm135 = _mm_fmadd_pd(__mv_vop417, __mv_vop418, __mv_accm135);	// latency = 5
  // stmt vectorized: y[148] += A[358] * x[147]
__mv_hi = _mm_shuffle_pd(__mv_accm135, __mv_accm135, 0x1);
__mv_accm135 = _mm_add_pd(__mv_accm135, __mv_hi);
y[148] = y[148]+__mv_accm135[0];;
  // stmt vectorized: y[148] += A[359] * x[149];
  __mv_accm136 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop420 = _mm256_maskload_pd(&A[((i0 + 0) + 360)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop421 = _mm256_maskload_pd(&x[((i0 + 0) + 148)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm136 = _mm256_fmadd_pd(__mv_vop420, __mv_vop421, __mv_accm136);	// latency = 5
    // stmt vectorized: y[0 * i0 + 149] += A[1 * i0 + 360] * x[1 * i0 + 148];
  }__mv_accm130 = _mm256_hadd_pd(__mv_accm130, __mv_accm132);	// latency = 6
__mv_accm130 = _mm256_add_pd(_mm256_blend_pd(__mv_accm130,__mv_accm134,0b1100), _mm256_permute2f128_pd(__mv_accm130,__mv_accm134,0x21));	// latency = 4
y[143] = y[143] + __mv_accm130[0];	// latency = 1
y[145] = y[145] + __mv_accm130[1];	// latency = 1
y[147] = y[147] + __mv_accm130[2];	// latency = 1
y[149] = y[149] + __mv_accm130[3];	// latency = 1

  __mv_vop423 = _mm_loadu_pd(&A[363]);	// latency = 1
  __mv_vop424 = _mm_i64gather_pd(&x[149], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm137 = _mm_fmadd_pd(__mv_vop423, __mv_vop424, __mv_accm137);	// latency = 5
  // stmt vectorized: y[150] += A[363] * x[149]
__mv_hi = _mm_shuffle_pd(__mv_accm137, __mv_accm137, 0x1);
__mv_accm137 = _mm_add_pd(__mv_accm137, __mv_hi);
y[150] = y[150]+__mv_accm137[0];;
  // stmt vectorized: y[150] += A[364] * x[151];
  __mv_accm138 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop426 = _mm256_maskload_pd(&A[((i0 + 0) + 365)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop427 = _mm256_maskload_pd(&x[((i0 + 0) + 150)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm138 = _mm256_fmadd_pd(__mv_vop426, __mv_vop427, __mv_accm138);	// latency = 5
    // stmt vectorized: y[0 * i0 + 151] += A[1 * i0 + 365] * x[1 * i0 + 150];
  }
  __mv_vop429 = _mm_loadu_pd(&A[368]);	// latency = 1
  __mv_vop430 = _mm_i64gather_pd(&x[151], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm139 = _mm_fmadd_pd(__mv_vop429, __mv_vop430, __mv_accm139);	// latency = 5
  // stmt vectorized: y[152] += A[368] * x[151]
__mv_hi = _mm_shuffle_pd(__mv_accm139, __mv_accm139, 0x1);
__mv_accm139 = _mm_add_pd(__mv_accm139, __mv_hi);
y[152] = y[152]+__mv_accm139[0];;
  // stmt vectorized: y[152] += A[369] * x[153];
  __mv_accm140 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop432 = _mm256_maskload_pd(&A[((i0 + 0) + 370)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop433 = _mm256_maskload_pd(&x[((i0 + 0) + 152)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm140 = _mm256_fmadd_pd(__mv_vop432, __mv_vop433, __mv_accm140);	// latency = 5
    // stmt vectorized: y[0 * i0 + 153] += A[1 * i0 + 370] * x[1 * i0 + 152];
  }
  __mv_vop435 = _mm_loadu_pd(&A[373]);	// latency = 1
  __mv_vop436 = _mm_i64gather_pd(&x[153], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm141 = _mm_fmadd_pd(__mv_vop435, __mv_vop436, __mv_accm141);	// latency = 5
  // stmt vectorized: y[154] += A[373] * x[153]
__mv_hi = _mm_shuffle_pd(__mv_accm141, __mv_accm141, 0x1);
__mv_accm141 = _mm_add_pd(__mv_accm141, __mv_hi);
y[154] = y[154]+__mv_accm141[0];;
  // stmt vectorized: y[154] += A[374] * x[155];
  __mv_accm142 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop438 = _mm256_maskload_pd(&A[((i0 + 0) + 375)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop439 = _mm256_maskload_pd(&x[((i0 + 0) + 154)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm142 = _mm256_fmadd_pd(__mv_vop438, __mv_vop439, __mv_accm142);	// latency = 5
    // stmt vectorized: y[0 * i0 + 155] += A[1 * i0 + 375] * x[1 * i0 + 154];
  }
  __mv_vop441 = _mm_loadu_pd(&A[378]);	// latency = 1
  __mv_vop442 = _mm_i64gather_pd(&x[155], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm143 = _mm_fmadd_pd(__mv_vop441, __mv_vop442, __mv_accm143);	// latency = 5
  // stmt vectorized: y[156] += A[378] * x[155]
__mv_hi = _mm_shuffle_pd(__mv_accm143, __mv_accm143, 0x1);
__mv_accm143 = _mm_add_pd(__mv_accm143, __mv_hi);
y[156] = y[156]+__mv_accm143[0];;
  // stmt vectorized: y[156] += A[379] * x[157];
  __mv_accm144 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop444 = _mm256_maskload_pd(&A[((i0 + 0) + 380)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop445 = _mm256_maskload_pd(&x[((i0 + 0) + 156)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm144 = _mm256_fmadd_pd(__mv_vop444, __mv_vop445, __mv_accm144);	// latency = 5
    // stmt vectorized: y[0 * i0 + 157] += A[1 * i0 + 380] * x[1 * i0 + 156];
  }__mv_accm138 = _mm256_hadd_pd(__mv_accm138, __mv_accm140);	// latency = 6
__mv_accm138 = _mm256_add_pd(_mm256_blend_pd(__mv_accm138,__mv_accm142,0b1100), _mm256_permute2f128_pd(__mv_accm138,__mv_accm142,0x21));	// latency = 4
y[151] = y[151] + __mv_accm138[0];	// latency = 1
y[153] = y[153] + __mv_accm138[1];	// latency = 1
y[155] = y[155] + __mv_accm138[2];	// latency = 1
y[157] = y[157] + __mv_accm138[3];	// latency = 1

  __mv_vop447 = _mm_loadu_pd(&A[383]);	// latency = 1
  __mv_vop448 = _mm_i64gather_pd(&x[157], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm145 = _mm_fmadd_pd(__mv_vop447, __mv_vop448, __mv_accm145);	// latency = 5
  // stmt vectorized: y[158] += A[383] * x[157]
__mv_hi = _mm_shuffle_pd(__mv_accm145, __mv_accm145, 0x1);
__mv_accm145 = _mm_add_pd(__mv_accm145, __mv_hi);
y[158] = y[158]+__mv_accm145[0];;
  // stmt vectorized: y[158] += A[384] * x[159];
  __mv_accm146 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop450 = _mm256_maskload_pd(&A[((i0 + 0) + 385)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop451 = _mm256_maskload_pd(&x[((i0 + 0) + 158)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm146 = _mm256_fmadd_pd(__mv_vop450, __mv_vop451, __mv_accm146);	// latency = 5
    // stmt vectorized: y[0 * i0 + 159] += A[1 * i0 + 385] * x[1 * i0 + 158];
  }
  __mv_vop453 = _mm_loadu_pd(&A[388]);	// latency = 1
  __mv_vop454 = _mm_i64gather_pd(&x[159], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm147 = _mm_fmadd_pd(__mv_vop453, __mv_vop454, __mv_accm147);	// latency = 5
  // stmt vectorized: y[160] += A[388] * x[159]
__mv_hi = _mm_shuffle_pd(__mv_accm147, __mv_accm147, 0x1);
__mv_accm147 = _mm_add_pd(__mv_accm147, __mv_hi);
y[160] = y[160]+__mv_accm147[0];;
  // stmt vectorized: y[160] += A[389] * x[161];
  __mv_accm148 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop456 = _mm256_maskload_pd(&A[((i0 + 0) + 390)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop457 = _mm256_maskload_pd(&x[((i0 + 0) + 160)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm148 = _mm256_fmadd_pd(__mv_vop456, __mv_vop457, __mv_accm148);	// latency = 5
    // stmt vectorized: y[0 * i0 + 161] += A[1 * i0 + 390] * x[1 * i0 + 160];
  }
  __mv_vop459 = _mm_loadu_pd(&A[393]);	// latency = 1
  __mv_vop460 = _mm_i64gather_pd(&x[161], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm149 = _mm_fmadd_pd(__mv_vop459, __mv_vop460, __mv_accm149);	// latency = 5
  // stmt vectorized: y[162] += A[393] * x[161]
__mv_hi = _mm_shuffle_pd(__mv_accm149, __mv_accm149, 0x1);
__mv_accm149 = _mm_add_pd(__mv_accm149, __mv_hi);
y[162] = y[162]+__mv_accm149[0];;
  // stmt vectorized: y[162] += A[394] * x[163];
  __mv_accm150 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop462 = _mm256_maskload_pd(&A[((i0 + 0) + 395)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop463 = _mm256_maskload_pd(&x[((i0 + 0) + 162)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm150 = _mm256_fmadd_pd(__mv_vop462, __mv_vop463, __mv_accm150);	// latency = 5
    // stmt vectorized: y[0 * i0 + 163] += A[1 * i0 + 395] * x[1 * i0 + 162];
  }
  __mv_vop465 = _mm_loadu_pd(&A[398]);	// latency = 1
  __mv_vop466 = _mm_i64gather_pd(&x[163], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm151 = _mm_fmadd_pd(__mv_vop465, __mv_vop466, __mv_accm151);	// latency = 5
  // stmt vectorized: y[164] += A[398] * x[163]
__mv_hi = _mm_shuffle_pd(__mv_accm151, __mv_accm151, 0x1);
__mv_accm151 = _mm_add_pd(__mv_accm151, __mv_hi);
y[164] = y[164]+__mv_accm151[0];;
  // stmt vectorized: y[164] += A[399] * x[165];
  __mv_accm152 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop468 = _mm256_maskload_pd(&A[((i0 + 0) + 400)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop469 = _mm256_maskload_pd(&x[((i0 + 0) + 164)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm152 = _mm256_fmadd_pd(__mv_vop468, __mv_vop469, __mv_accm152);	// latency = 5
    // stmt vectorized: y[0 * i0 + 165] += A[1 * i0 + 400] * x[1 * i0 + 164];
  }__mv_accm146 = _mm256_hadd_pd(__mv_accm146, __mv_accm148);	// latency = 6
__mv_accm146 = _mm256_add_pd(_mm256_blend_pd(__mv_accm146,__mv_accm150,0b1100), _mm256_permute2f128_pd(__mv_accm146,__mv_accm150,0x21));	// latency = 4
y[159] = y[159] + __mv_accm146[0];	// latency = 1
y[161] = y[161] + __mv_accm146[1];	// latency = 1
y[163] = y[163] + __mv_accm146[2];	// latency = 1
y[165] = y[165] + __mv_accm146[3];	// latency = 1

  __mv_vop471 = _mm_loadu_pd(&A[403]);	// latency = 1
  __mv_vop472 = _mm_i64gather_pd(&x[165], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm153 = _mm_fmadd_pd(__mv_vop471, __mv_vop472, __mv_accm153);	// latency = 5
  // stmt vectorized: y[166] += A[403] * x[165]
__mv_hi = _mm_shuffle_pd(__mv_accm153, __mv_accm153, 0x1);
__mv_accm153 = _mm_add_pd(__mv_accm153, __mv_hi);
y[166] = y[166]+__mv_accm153[0];;
  // stmt vectorized: y[166] += A[404] * x[167];
  __mv_accm154 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop474 = _mm256_maskload_pd(&A[((i0 + 0) + 405)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop475 = _mm256_maskload_pd(&x[((i0 + 0) + 166)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm154 = _mm256_fmadd_pd(__mv_vop474, __mv_vop475, __mv_accm154);	// latency = 5
    // stmt vectorized: y[0 * i0 + 167] += A[1 * i0 + 405] * x[1 * i0 + 166];
  }
  __mv_vop477 = _mm_loadu_pd(&A[408]);	// latency = 1
  __mv_vop478 = _mm_i64gather_pd(&x[167], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm155 = _mm_fmadd_pd(__mv_vop477, __mv_vop478, __mv_accm155);	// latency = 5
  // stmt vectorized: y[168] += A[408] * x[167]
__mv_hi = _mm_shuffle_pd(__mv_accm155, __mv_accm155, 0x1);
__mv_accm155 = _mm_add_pd(__mv_accm155, __mv_hi);
y[168] = y[168]+__mv_accm155[0];;
  // stmt vectorized: y[168] += A[409] * x[169];
  __mv_accm156 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop480 = _mm256_maskload_pd(&A[((i0 + 0) + 410)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop481 = _mm256_maskload_pd(&x[((i0 + 0) + 168)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm156 = _mm256_fmadd_pd(__mv_vop480, __mv_vop481, __mv_accm156);	// latency = 5
    // stmt vectorized: y[0 * i0 + 169] += A[1 * i0 + 410] * x[1 * i0 + 168];
  }
  __mv_vop483 = _mm_loadu_pd(&A[413]);	// latency = 1
  __mv_vop484 = _mm_i64gather_pd(&x[169], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm157 = _mm_fmadd_pd(__mv_vop483, __mv_vop484, __mv_accm157);	// latency = 5
  // stmt vectorized: y[170] += A[413] * x[169]
__mv_hi = _mm_shuffle_pd(__mv_accm157, __mv_accm157, 0x1);
__mv_accm157 = _mm_add_pd(__mv_accm157, __mv_hi);
y[170] = y[170]+__mv_accm157[0];;
  // stmt vectorized: y[170] += A[414] * x[171];
  __mv_accm158 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop486 = _mm256_maskload_pd(&A[((i0 + 0) + 415)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop487 = _mm256_maskload_pd(&x[((i0 + 0) + 170)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm158 = _mm256_fmadd_pd(__mv_vop486, __mv_vop487, __mv_accm158);	// latency = 5
    // stmt vectorized: y[0 * i0 + 171] += A[1 * i0 + 415] * x[1 * i0 + 170];
  }
  __mv_vop489 = _mm_loadu_pd(&A[418]);	// latency = 1
  __mv_vop490 = _mm_i64gather_pd(&x[171], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm159 = _mm_fmadd_pd(__mv_vop489, __mv_vop490, __mv_accm159);	// latency = 5
  // stmt vectorized: y[172] += A[418] * x[171]
__mv_hi = _mm_shuffle_pd(__mv_accm159, __mv_accm159, 0x1);
__mv_accm159 = _mm_add_pd(__mv_accm159, __mv_hi);
y[172] = y[172]+__mv_accm159[0];;
  // stmt vectorized: y[172] += A[419] * x[173];
  __mv_accm160 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop492 = _mm256_maskload_pd(&A[((i0 + 0) + 420)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop493 = _mm256_maskload_pd(&x[((i0 + 0) + 172)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm160 = _mm256_fmadd_pd(__mv_vop492, __mv_vop493, __mv_accm160);	// latency = 5
    // stmt vectorized: y[0 * i0 + 173] += A[1 * i0 + 420] * x[1 * i0 + 172];
  }__mv_accm154 = _mm256_hadd_pd(__mv_accm154, __mv_accm156);	// latency = 6
__mv_accm154 = _mm256_add_pd(_mm256_blend_pd(__mv_accm154,__mv_accm158,0b1100), _mm256_permute2f128_pd(__mv_accm154,__mv_accm158,0x21));	// latency = 4
y[167] = y[167] + __mv_accm154[0];	// latency = 1
y[169] = y[169] + __mv_accm154[1];	// latency = 1
y[171] = y[171] + __mv_accm154[2];	// latency = 1
y[173] = y[173] + __mv_accm154[3];	// latency = 1

  __mv_vop495 = _mm256_loadu_pd(&A[423]);	// latency = 1
  __mv_vop496 = _mm256_i64gather_pd(&x[1], _mm256_set_epi64x(4014, 174, 172, 0), 8);	// latency = 10
  __mv_accm161 = _mm256_fmadd_pd(__mv_vop495, __mv_vop496, __mv_accm161);	// latency = 5
  // stmt vectorized: y[174] += A[423] * x[1]
__mv_lo256 = _mm256_castpd256_pd128(__mv_accm161);
__mv_hi256 = _mm256_extractf128_pd(__mv_accm161, 0x1);
__mv_lo256 = _mm_add_pd(__mv_lo256, __mv_hi256);
__mv_hi256 = _mm_shuffle_pd(__mv_lo256, __mv_lo256, 0x1);
__mv_lo256 = _mm_add_pd(__mv_lo256, __mv_hi256);
y[174] = y[174]+__mv_lo256[0];;
  // stmt vectorized: y[174] += A[424] * x[173];
  // stmt vectorized: y[174] += A[425] * x[175];
  // stmt vectorized: y[174] += A[426] * x[4015];
  __mv_accm162 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop498 = _mm256_maskload_pd(&A[((i0 + 0) + 427)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop499 = _mm256_maskload_pd(&x[((i0 + 0) + 174)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm162 = _mm256_fmadd_pd(__mv_vop498, __mv_vop499, __mv_accm162);	// latency = 5
    // stmt vectorized: y[0 * i0 + 175] += A[1 * i0 + 427] * x[1 * i0 + 174];
  }
  __mv_vop501 = _mm_loadu_pd(&A[430]);	// latency = 1
  __mv_vop502 = _mm_i64gather_pd(&x[175], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm163 = _mm_fmadd_pd(__mv_vop501, __mv_vop502, __mv_accm163);	// latency = 5
  // stmt vectorized: y[176] += A[430] * x[175]
__mv_hi = _mm_shuffle_pd(__mv_accm163, __mv_accm163, 0x1);
__mv_accm163 = _mm_add_pd(__mv_accm163, __mv_hi);
y[176] = y[176]+__mv_accm163[0];;
  // stmt vectorized: y[176] += A[431] * x[177];
  __mv_accm164 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop504 = _mm256_maskload_pd(&A[((i0 + 0) + 432)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop505 = _mm256_maskload_pd(&x[((i0 + 0) + 176)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm164 = _mm256_fmadd_pd(__mv_vop504, __mv_vop505, __mv_accm164);	// latency = 5
    // stmt vectorized: y[0 * i0 + 177] += A[1 * i0 + 432] * x[1 * i0 + 176];
  }
  __mv_vop507 = _mm_loadu_pd(&A[435]);	// latency = 1
  __mv_vop508 = _mm_i64gather_pd(&x[177], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm165 = _mm_fmadd_pd(__mv_vop507, __mv_vop508, __mv_accm165);	// latency = 5
  // stmt vectorized: y[178] += A[435] * x[177]
__mv_hi = _mm_shuffle_pd(__mv_accm165, __mv_accm165, 0x1);
__mv_accm165 = _mm_add_pd(__mv_accm165, __mv_hi);
y[178] = y[178]+__mv_accm165[0];;
  // stmt vectorized: y[178] += A[436] * x[179];
  __mv_accm166 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop510 = _mm256_maskload_pd(&A[((i0 + 0) + 437)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop511 = _mm256_maskload_pd(&x[((i0 + 0) + 178)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm166 = _mm256_fmadd_pd(__mv_vop510, __mv_vop511, __mv_accm166);	// latency = 5
    // stmt vectorized: y[0 * i0 + 179] += A[1 * i0 + 437] * x[1 * i0 + 178];
  }
  __mv_vop513 = _mm_loadu_pd(&A[440]);	// latency = 1
  __mv_vop514 = _mm_i64gather_pd(&x[179], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm167 = _mm_fmadd_pd(__mv_vop513, __mv_vop514, __mv_accm167);	// latency = 5
  // stmt vectorized: y[180] += A[440] * x[179]
__mv_hi = _mm_shuffle_pd(__mv_accm167, __mv_accm167, 0x1);
__mv_accm167 = _mm_add_pd(__mv_accm167, __mv_hi);
y[180] = y[180]+__mv_accm167[0];;
  // stmt vectorized: y[180] += A[441] * x[181];
  __mv_accm168 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop516 = _mm256_maskload_pd(&A[((i0 + 0) + 442)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop517 = _mm256_maskload_pd(&x[((i0 + 0) + 180)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm168 = _mm256_fmadd_pd(__mv_vop516, __mv_vop517, __mv_accm168);	// latency = 5
    // stmt vectorized: y[0 * i0 + 181] += A[1 * i0 + 442] * x[1 * i0 + 180];
  }__mv_accm162 = _mm256_hadd_pd(__mv_accm162, __mv_accm164);	// latency = 6
__mv_accm162 = _mm256_add_pd(_mm256_blend_pd(__mv_accm162,__mv_accm166,0b1100), _mm256_permute2f128_pd(__mv_accm162,__mv_accm166,0x21));	// latency = 4
y[175] = y[175] + __mv_accm162[0];	// latency = 1
y[177] = y[177] + __mv_accm162[1];	// latency = 1
y[179] = y[179] + __mv_accm162[2];	// latency = 1
y[181] = y[181] + __mv_accm162[3];	// latency = 1

  __mv_vop519 = _mm_loadu_pd(&A[445]);	// latency = 1
  __mv_vop520 = _mm_i64gather_pd(&x[181], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm169 = _mm_fmadd_pd(__mv_vop519, __mv_vop520, __mv_accm169);	// latency = 5
  // stmt vectorized: y[182] += A[445] * x[181]
__mv_hi = _mm_shuffle_pd(__mv_accm169, __mv_accm169, 0x1);
__mv_accm169 = _mm_add_pd(__mv_accm169, __mv_hi);
y[182] = y[182]+__mv_accm169[0];;
  // stmt vectorized: y[182] += A[446] * x[183];
  __mv_accm170 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop522 = _mm256_maskload_pd(&A[((i0 + 0) + 447)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop523 = _mm256_maskload_pd(&x[((i0 + 0) + 182)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm170 = _mm256_fmadd_pd(__mv_vop522, __mv_vop523, __mv_accm170);	// latency = 5
    // stmt vectorized: y[0 * i0 + 183] += A[1 * i0 + 447] * x[1 * i0 + 182];
  }
  __mv_vop525 = _mm_loadu_pd(&A[450]);	// latency = 1
  __mv_vop526 = _mm_i64gather_pd(&x[183], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm171 = _mm_fmadd_pd(__mv_vop525, __mv_vop526, __mv_accm171);	// latency = 5
  // stmt vectorized: y[184] += A[450] * x[183]
__mv_hi = _mm_shuffle_pd(__mv_accm171, __mv_accm171, 0x1);
__mv_accm171 = _mm_add_pd(__mv_accm171, __mv_hi);
y[184] = y[184]+__mv_accm171[0];;
  // stmt vectorized: y[184] += A[451] * x[185];
  __mv_accm172 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop528 = _mm256_maskload_pd(&A[((i0 + 0) + 452)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop529 = _mm256_maskload_pd(&x[((i0 + 0) + 184)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm172 = _mm256_fmadd_pd(__mv_vop528, __mv_vop529, __mv_accm172);	// latency = 5
    // stmt vectorized: y[0 * i0 + 185] += A[1 * i0 + 452] * x[1 * i0 + 184];
  }
  __mv_vop531 = _mm_loadu_pd(&A[455]);	// latency = 1
  __mv_vop532 = _mm_i64gather_pd(&x[185], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm173 = _mm_fmadd_pd(__mv_vop531, __mv_vop532, __mv_accm173);	// latency = 5
  // stmt vectorized: y[186] += A[455] * x[185]
__mv_hi = _mm_shuffle_pd(__mv_accm173, __mv_accm173, 0x1);
__mv_accm173 = _mm_add_pd(__mv_accm173, __mv_hi);
y[186] = y[186]+__mv_accm173[0];;
  // stmt vectorized: y[186] += A[456] * x[187];
  __mv_accm174 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop534 = _mm256_maskload_pd(&A[((i0 + 0) + 457)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop535 = _mm256_maskload_pd(&x[((i0 + 0) + 186)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm174 = _mm256_fmadd_pd(__mv_vop534, __mv_vop535, __mv_accm174);	// latency = 5
    // stmt vectorized: y[0 * i0 + 187] += A[1 * i0 + 457] * x[1 * i0 + 186];
  }
  __mv_vop537 = _mm_loadu_pd(&A[460]);	// latency = 1
  __mv_vop538 = _mm_i64gather_pd(&x[187], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm175 = _mm_fmadd_pd(__mv_vop537, __mv_vop538, __mv_accm175);	// latency = 5
  // stmt vectorized: y[188] += A[460] * x[187]
__mv_hi = _mm_shuffle_pd(__mv_accm175, __mv_accm175, 0x1);
__mv_accm175 = _mm_add_pd(__mv_accm175, __mv_hi);
y[188] = y[188]+__mv_accm175[0];;
  // stmt vectorized: y[188] += A[461] * x[189];
  __mv_accm176 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop540 = _mm256_maskload_pd(&A[((i0 + 0) + 462)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop541 = _mm256_maskload_pd(&x[((i0 + 0) + 188)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm176 = _mm256_fmadd_pd(__mv_vop540, __mv_vop541, __mv_accm176);	// latency = 5
    // stmt vectorized: y[0 * i0 + 189] += A[1 * i0 + 462] * x[1 * i0 + 188];
  }__mv_accm170 = _mm256_hadd_pd(__mv_accm170, __mv_accm172);	// latency = 6
__mv_accm170 = _mm256_add_pd(_mm256_blend_pd(__mv_accm170,__mv_accm174,0b1100), _mm256_permute2f128_pd(__mv_accm170,__mv_accm174,0x21));	// latency = 4
y[183] = y[183] + __mv_accm170[0];	// latency = 1
y[185] = y[185] + __mv_accm170[1];	// latency = 1
y[187] = y[187] + __mv_accm170[2];	// latency = 1
y[189] = y[189] + __mv_accm170[3];	// latency = 1

  __mv_vop543 = _mm_loadu_pd(&A[465]);	// latency = 1
  __mv_vop544 = _mm_i64gather_pd(&x[189], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm177 = _mm_fmadd_pd(__mv_vop543, __mv_vop544, __mv_accm177);	// latency = 5
  // stmt vectorized: y[190] += A[465] * x[189]
__mv_hi = _mm_shuffle_pd(__mv_accm177, __mv_accm177, 0x1);
__mv_accm177 = _mm_add_pd(__mv_accm177, __mv_hi);
y[190] = y[190]+__mv_accm177[0];;
  // stmt vectorized: y[190] += A[466] * x[191];
  __mv_accm178 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop546 = _mm256_maskload_pd(&A[((i0 + 0) + 467)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop547 = _mm256_maskload_pd(&x[((i0 + 0) + 190)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm178 = _mm256_fmadd_pd(__mv_vop546, __mv_vop547, __mv_accm178);	// latency = 5
    // stmt vectorized: y[0 * i0 + 191] += A[1 * i0 + 467] * x[1 * i0 + 190];
  }
  __mv_vop549 = _mm_loadu_pd(&A[470]);	// latency = 1
  __mv_vop550 = _mm_i64gather_pd(&x[191], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm179 = _mm_fmadd_pd(__mv_vop549, __mv_vop550, __mv_accm179);	// latency = 5
  // stmt vectorized: y[192] += A[470] * x[191]
__mv_hi = _mm_shuffle_pd(__mv_accm179, __mv_accm179, 0x1);
__mv_accm179 = _mm_add_pd(__mv_accm179, __mv_hi);
y[192] = y[192]+__mv_accm179[0];;
  // stmt vectorized: y[192] += A[471] * x[193];
  __mv_accm180 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop552 = _mm256_maskload_pd(&A[((i0 + 0) + 472)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop553 = _mm256_maskload_pd(&x[((i0 + 0) + 192)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm180 = _mm256_fmadd_pd(__mv_vop552, __mv_vop553, __mv_accm180);	// latency = 5
    // stmt vectorized: y[0 * i0 + 193] += A[1 * i0 + 472] * x[1 * i0 + 192];
  }
  __mv_vop555 = _mm_loadu_pd(&A[475]);	// latency = 1
  __mv_vop556 = _mm_i64gather_pd(&x[193], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm181 = _mm_fmadd_pd(__mv_vop555, __mv_vop556, __mv_accm181);	// latency = 5
  // stmt vectorized: y[194] += A[475] * x[193]
__mv_hi = _mm_shuffle_pd(__mv_accm181, __mv_accm181, 0x1);
__mv_accm181 = _mm_add_pd(__mv_accm181, __mv_hi);
y[194] = y[194]+__mv_accm181[0];;
  // stmt vectorized: y[194] += A[476] * x[195];
  __mv_accm182 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop558 = _mm256_maskload_pd(&A[((i0 + 0) + 477)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop559 = _mm256_maskload_pd(&x[((i0 + 0) + 194)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm182 = _mm256_fmadd_pd(__mv_vop558, __mv_vop559, __mv_accm182);	// latency = 5
    // stmt vectorized: y[0 * i0 + 195] += A[1 * i0 + 477] * x[1 * i0 + 194];
  }
  __mv_vop561 = _mm_loadu_pd(&A[480]);	// latency = 1
  __mv_vop562 = _mm_i64gather_pd(&x[195], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm183 = _mm_fmadd_pd(__mv_vop561, __mv_vop562, __mv_accm183);	// latency = 5
  // stmt vectorized: y[196] += A[480] * x[195]
__mv_hi = _mm_shuffle_pd(__mv_accm183, __mv_accm183, 0x1);
__mv_accm183 = _mm_add_pd(__mv_accm183, __mv_hi);
y[196] = y[196]+__mv_accm183[0];;
  // stmt vectorized: y[196] += A[481] * x[197];
  __mv_accm184 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop564 = _mm256_maskload_pd(&A[((i0 + 0) + 482)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop565 = _mm256_maskload_pd(&x[((i0 + 0) + 196)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm184 = _mm256_fmadd_pd(__mv_vop564, __mv_vop565, __mv_accm184);	// latency = 5
    // stmt vectorized: y[0 * i0 + 197] += A[1 * i0 + 482] * x[1 * i0 + 196];
  }__mv_accm178 = _mm256_hadd_pd(__mv_accm178, __mv_accm180);	// latency = 6
__mv_accm178 = _mm256_add_pd(_mm256_blend_pd(__mv_accm178,__mv_accm182,0b1100), _mm256_permute2f128_pd(__mv_accm178,__mv_accm182,0x21));	// latency = 4
y[191] = y[191] + __mv_accm178[0];	// latency = 1
y[193] = y[193] + __mv_accm178[1];	// latency = 1
y[195] = y[195] + __mv_accm178[2];	// latency = 1
y[197] = y[197] + __mv_accm178[3];	// latency = 1

  __mv_vop567 = _mm_loadu_pd(&A[485]);	// latency = 1
  __mv_vop568 = _mm_i64gather_pd(&x[197], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm185 = _mm_fmadd_pd(__mv_vop567, __mv_vop568, __mv_accm185);	// latency = 5
  // stmt vectorized: y[198] += A[485] * x[197]
__mv_hi = _mm_shuffle_pd(__mv_accm185, __mv_accm185, 0x1);
__mv_accm185 = _mm_add_pd(__mv_accm185, __mv_hi);
y[198] = y[198]+__mv_accm185[0];;
  // stmt vectorized: y[198] += A[486] * x[199];
  __mv_accm186 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop570 = _mm256_maskload_pd(&A[((i0 + 0) + 487)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop571 = _mm256_maskload_pd(&x[((i0 + 0) + 198)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm186 = _mm256_fmadd_pd(__mv_vop570, __mv_vop571, __mv_accm186);	// latency = 5
    // stmt vectorized: y[0 * i0 + 199] += A[1 * i0 + 487] * x[1 * i0 + 198];
  }
  __mv_vop573 = _mm_loadu_pd(&A[490]);	// latency = 1
  __mv_vop574 = _mm_i64gather_pd(&x[199], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm187 = _mm_fmadd_pd(__mv_vop573, __mv_vop574, __mv_accm187);	// latency = 5
  // stmt vectorized: y[200] += A[490] * x[199]
__mv_hi = _mm_shuffle_pd(__mv_accm187, __mv_accm187, 0x1);
__mv_accm187 = _mm_add_pd(__mv_accm187, __mv_hi);
y[200] = y[200]+__mv_accm187[0];;
  // stmt vectorized: y[200] += A[491] * x[201];
  __mv_accm188 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop576 = _mm256_maskload_pd(&A[((i0 + 0) + 492)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop577 = _mm256_maskload_pd(&x[((i0 + 0) + 200)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm188 = _mm256_fmadd_pd(__mv_vop576, __mv_vop577, __mv_accm188);	// latency = 5
    // stmt vectorized: y[0 * i0 + 201] += A[1 * i0 + 492] * x[1 * i0 + 200];
  }
  __mv_vop579 = _mm_loadu_pd(&A[495]);	// latency = 1
  __mv_vop580 = _mm_i64gather_pd(&x[201], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm189 = _mm_fmadd_pd(__mv_vop579, __mv_vop580, __mv_accm189);	// latency = 5
  // stmt vectorized: y[202] += A[495] * x[201]
__mv_hi = _mm_shuffle_pd(__mv_accm189, __mv_accm189, 0x1);
__mv_accm189 = _mm_add_pd(__mv_accm189, __mv_hi);
y[202] = y[202]+__mv_accm189[0];;
  // stmt vectorized: y[202] += A[496] * x[203];
  __mv_accm190 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop582 = _mm256_maskload_pd(&A[((i0 + 0) + 497)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop583 = _mm256_maskload_pd(&x[((i0 + 0) + 202)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm190 = _mm256_fmadd_pd(__mv_vop582, __mv_vop583, __mv_accm190);	// latency = 5
    // stmt vectorized: y[0 * i0 + 203] += A[1 * i0 + 497] * x[1 * i0 + 202];
  }
  __mv_vop585 = _mm_loadu_pd(&A[500]);	// latency = 1
  __mv_vop586 = _mm_i64gather_pd(&x[203], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm191 = _mm_fmadd_pd(__mv_vop585, __mv_vop586, __mv_accm191);	// latency = 5
  // stmt vectorized: y[204] += A[500] * x[203]
__mv_hi = _mm_shuffle_pd(__mv_accm191, __mv_accm191, 0x1);
__mv_accm191 = _mm_add_pd(__mv_accm191, __mv_hi);
y[204] = y[204]+__mv_accm191[0];;
  // stmt vectorized: y[204] += A[501] * x[205];
  __mv_accm192 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop588 = _mm256_maskload_pd(&A[((i0 + 0) + 502)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop589 = _mm256_maskload_pd(&x[((i0 + 0) + 204)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm192 = _mm256_fmadd_pd(__mv_vop588, __mv_vop589, __mv_accm192);	// latency = 5
    // stmt vectorized: y[0 * i0 + 205] += A[1 * i0 + 502] * x[1 * i0 + 204];
  }__mv_accm186 = _mm256_hadd_pd(__mv_accm186, __mv_accm188);	// latency = 6
__mv_accm186 = _mm256_add_pd(_mm256_blend_pd(__mv_accm186,__mv_accm190,0b1100), _mm256_permute2f128_pd(__mv_accm186,__mv_accm190,0x21));	// latency = 4
y[199] = y[199] + __mv_accm186[0];	// latency = 1
y[201] = y[201] + __mv_accm186[1];	// latency = 1
y[203] = y[203] + __mv_accm186[2];	// latency = 1
y[205] = y[205] + __mv_accm186[3];	// latency = 1

  __mv_vop591 = _mm_loadu_pd(&A[505]);	// latency = 1
  __mv_vop592 = _mm_i64gather_pd(&x[205], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm193 = _mm_fmadd_pd(__mv_vop591, __mv_vop592, __mv_accm193);	// latency = 5
  // stmt vectorized: y[206] += A[505] * x[205]
__mv_hi = _mm_shuffle_pd(__mv_accm193, __mv_accm193, 0x1);
__mv_accm193 = _mm_add_pd(__mv_accm193, __mv_hi);
y[206] = y[206]+__mv_accm193[0];;
  // stmt vectorized: y[206] += A[506] * x[207];
  __mv_accm194 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop594 = _mm256_maskload_pd(&A[((i0 + 0) + 507)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop595 = _mm256_maskload_pd(&x[((i0 + 0) + 206)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm194 = _mm256_fmadd_pd(__mv_vop594, __mv_vop595, __mv_accm194);	// latency = 5
    // stmt vectorized: y[0 * i0 + 207] += A[1 * i0 + 507] * x[1 * i0 + 206];
  }
  __mv_vop597 = _mm_loadu_pd(&A[510]);	// latency = 1
  __mv_vop598 = _mm_i64gather_pd(&x[207], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm195 = _mm_fmadd_pd(__mv_vop597, __mv_vop598, __mv_accm195);	// latency = 5
  // stmt vectorized: y[208] += A[510] * x[207]
__mv_hi = _mm_shuffle_pd(__mv_accm195, __mv_accm195, 0x1);
__mv_accm195 = _mm_add_pd(__mv_accm195, __mv_hi);
y[208] = y[208]+__mv_accm195[0];;
  // stmt vectorized: y[208] += A[511] * x[209];
  __mv_accm196 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop600 = _mm256_maskload_pd(&A[((i0 + 0) + 512)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop601 = _mm256_maskload_pd(&x[((i0 + 0) + 208)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm196 = _mm256_fmadd_pd(__mv_vop600, __mv_vop601, __mv_accm196);	// latency = 5
    // stmt vectorized: y[0 * i0 + 209] += A[1 * i0 + 512] * x[1 * i0 + 208];
  }
  __mv_vop603 = _mm_loadu_pd(&A[515]);	// latency = 1
  __mv_vop604 = _mm_i64gather_pd(&x[209], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm197 = _mm_fmadd_pd(__mv_vop603, __mv_vop604, __mv_accm197);	// latency = 5
  // stmt vectorized: y[210] += A[515] * x[209]
__mv_hi = _mm_shuffle_pd(__mv_accm197, __mv_accm197, 0x1);
__mv_accm197 = _mm_add_pd(__mv_accm197, __mv_hi);
y[210] = y[210]+__mv_accm197[0];;
  // stmt vectorized: y[210] += A[516] * x[211];
  __mv_accm198 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop606 = _mm256_maskload_pd(&A[((i0 + 0) + 517)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop607 = _mm256_maskload_pd(&x[((i0 + 0) + 210)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm198 = _mm256_fmadd_pd(__mv_vop606, __mv_vop607, __mv_accm198);	// latency = 5
    // stmt vectorized: y[0 * i0 + 211] += A[1 * i0 + 517] * x[1 * i0 + 210];
  }
  __mv_vop609 = _mm_loadu_pd(&A[520]);	// latency = 1
  __mv_vop610 = _mm_i64gather_pd(&x[211], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm199 = _mm_fmadd_pd(__mv_vop609, __mv_vop610, __mv_accm199);	// latency = 5
  // stmt vectorized: y[212] += A[520] * x[211]
__mv_hi = _mm_shuffle_pd(__mv_accm199, __mv_accm199, 0x1);
__mv_accm199 = _mm_add_pd(__mv_accm199, __mv_hi);
y[212] = y[212]+__mv_accm199[0];;
  // stmt vectorized: y[212] += A[521] * x[213];
  __mv_accm200 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop612 = _mm256_maskload_pd(&A[((i0 + 0) + 522)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop613 = _mm256_maskload_pd(&x[((i0 + 0) + 212)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm200 = _mm256_fmadd_pd(__mv_vop612, __mv_vop613, __mv_accm200);	// latency = 5
    // stmt vectorized: y[0 * i0 + 213] += A[1 * i0 + 522] * x[1 * i0 + 212];
  }__mv_accm194 = _mm256_hadd_pd(__mv_accm194, __mv_accm196);	// latency = 6
__mv_accm194 = _mm256_add_pd(_mm256_blend_pd(__mv_accm194,__mv_accm198,0b1100), _mm256_permute2f128_pd(__mv_accm194,__mv_accm198,0x21));	// latency = 4
y[207] = y[207] + __mv_accm194[0];	// latency = 1
y[209] = y[209] + __mv_accm194[1];	// latency = 1
y[211] = y[211] + __mv_accm194[2];	// latency = 1
y[213] = y[213] + __mv_accm194[3];	// latency = 1

  __mv_vop615 = _mm_loadu_pd(&A[525]);	// latency = 1
  __mv_vop616 = _mm_i64gather_pd(&x[213], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm201 = _mm_fmadd_pd(__mv_vop615, __mv_vop616, __mv_accm201);	// latency = 5
  // stmt vectorized: y[214] += A[525] * x[213]
__mv_hi = _mm_shuffle_pd(__mv_accm201, __mv_accm201, 0x1);
__mv_accm201 = _mm_add_pd(__mv_accm201, __mv_hi);
y[214] = y[214]+__mv_accm201[0];;
  // stmt vectorized: y[214] += A[526] * x[215];
  __mv_accm202 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop618 = _mm256_maskload_pd(&A[((i0 + 0) + 527)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop619 = _mm256_maskload_pd(&x[((i0 + 0) + 214)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm202 = _mm256_fmadd_pd(__mv_vop618, __mv_vop619, __mv_accm202);	// latency = 5
    // stmt vectorized: y[0 * i0 + 215] += A[1 * i0 + 527] * x[1 * i0 + 214];
  }
  __mv_vop621 = _mm_loadu_pd(&A[530]);	// latency = 1
  __mv_vop622 = _mm_i64gather_pd(&x[215], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm203 = _mm_fmadd_pd(__mv_vop621, __mv_vop622, __mv_accm203);	// latency = 5
  // stmt vectorized: y[216] += A[530] * x[215]
__mv_hi = _mm_shuffle_pd(__mv_accm203, __mv_accm203, 0x1);
__mv_accm203 = _mm_add_pd(__mv_accm203, __mv_hi);
y[216] = y[216]+__mv_accm203[0];;
  // stmt vectorized: y[216] += A[531] * x[217];
  __mv_accm204 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop624 = _mm256_maskload_pd(&A[((i0 + 0) + 532)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop625 = _mm256_maskload_pd(&x[((i0 + 0) + 216)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm204 = _mm256_fmadd_pd(__mv_vop624, __mv_vop625, __mv_accm204);	// latency = 5
    // stmt vectorized: y[0 * i0 + 217] += A[1 * i0 + 532] * x[1 * i0 + 216];
  }
  __mv_vop627 = _mm_loadu_pd(&A[535]);	// latency = 1
  __mv_vop628 = _mm_i64gather_pd(&x[217], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm205 = _mm_fmadd_pd(__mv_vop627, __mv_vop628, __mv_accm205);	// latency = 5
  // stmt vectorized: y[218] += A[535] * x[217]
__mv_hi = _mm_shuffle_pd(__mv_accm205, __mv_accm205, 0x1);
__mv_accm205 = _mm_add_pd(__mv_accm205, __mv_hi);
y[218] = y[218]+__mv_accm205[0];;
  // stmt vectorized: y[218] += A[536] * x[219];
  __mv_accm206 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop630 = _mm256_maskload_pd(&A[((i0 + 0) + 537)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop631 = _mm256_maskload_pd(&x[((i0 + 0) + 218)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm206 = _mm256_fmadd_pd(__mv_vop630, __mv_vop631, __mv_accm206);	// latency = 5
    // stmt vectorized: y[0 * i0 + 219] += A[1 * i0 + 537] * x[1 * i0 + 218];
  }
  __mv_vop633 = _mm_loadu_pd(&A[540]);	// latency = 1
  __mv_vop634 = _mm_i64gather_pd(&x[219], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm207 = _mm_fmadd_pd(__mv_vop633, __mv_vop634, __mv_accm207);	// latency = 5
  // stmt vectorized: y[220] += A[540] * x[219]
__mv_hi = _mm_shuffle_pd(__mv_accm207, __mv_accm207, 0x1);
__mv_accm207 = _mm_add_pd(__mv_accm207, __mv_hi);
y[220] = y[220]+__mv_accm207[0];;
  // stmt vectorized: y[220] += A[541] * x[221];
  __mv_accm208 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop636 = _mm256_maskload_pd(&A[((i0 + 0) + 542)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop637 = _mm256_maskload_pd(&x[((i0 + 0) + 220)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm208 = _mm256_fmadd_pd(__mv_vop636, __mv_vop637, __mv_accm208);	// latency = 5
    // stmt vectorized: y[0 * i0 + 221] += A[1 * i0 + 542] * x[1 * i0 + 220];
  }__mv_accm202 = _mm256_hadd_pd(__mv_accm202, __mv_accm204);	// latency = 6
__mv_accm202 = _mm256_add_pd(_mm256_blend_pd(__mv_accm202,__mv_accm206,0b1100), _mm256_permute2f128_pd(__mv_accm202,__mv_accm206,0x21));	// latency = 4
y[215] = y[215] + __mv_accm202[0];	// latency = 1
y[217] = y[217] + __mv_accm202[1];	// latency = 1
y[219] = y[219] + __mv_accm202[2];	// latency = 1
y[221] = y[221] + __mv_accm202[3];	// latency = 1

  __mv_vop639 = _mm_loadu_pd(&A[545]);	// latency = 1
  __mv_vop640 = _mm_i64gather_pd(&x[221], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm209 = _mm_fmadd_pd(__mv_vop639, __mv_vop640, __mv_accm209);	// latency = 5
  // stmt vectorized: y[222] += A[545] * x[221]
__mv_hi = _mm_shuffle_pd(__mv_accm209, __mv_accm209, 0x1);
__mv_accm209 = _mm_add_pd(__mv_accm209, __mv_hi);
y[222] = y[222]+__mv_accm209[0];;
  // stmt vectorized: y[222] += A[546] * x[223];
  __mv_accm210 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop642 = _mm256_maskload_pd(&A[((i0 + 0) + 547)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop643 = _mm256_maskload_pd(&x[((i0 + 0) + 222)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm210 = _mm256_fmadd_pd(__mv_vop642, __mv_vop643, __mv_accm210);	// latency = 5
    // stmt vectorized: y[0 * i0 + 223] += A[1 * i0 + 547] * x[1 * i0 + 222];
  }
  __mv_vop645 = _mm_loadu_pd(&A[550]);	// latency = 1
  __mv_vop646 = _mm_i64gather_pd(&x[223], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm211 = _mm_fmadd_pd(__mv_vop645, __mv_vop646, __mv_accm211);	// latency = 5
  // stmt vectorized: y[224] += A[550] * x[223]
__mv_hi = _mm_shuffle_pd(__mv_accm211, __mv_accm211, 0x1);
__mv_accm211 = _mm_add_pd(__mv_accm211, __mv_hi);
y[224] = y[224]+__mv_accm211[0];;
  // stmt vectorized: y[224] += A[551] * x[225];
  __mv_accm212 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop648 = _mm256_maskload_pd(&A[((i0 + 0) + 552)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop649 = _mm256_maskload_pd(&x[((i0 + 0) + 224)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm212 = _mm256_fmadd_pd(__mv_vop648, __mv_vop649, __mv_accm212);	// latency = 5
    // stmt vectorized: y[0 * i0 + 225] += A[1 * i0 + 552] * x[1 * i0 + 224];
  }
  __mv_vop651 = _mm_loadu_pd(&A[555]);	// latency = 1
  __mv_vop652 = _mm_i64gather_pd(&x[225], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm213 = _mm_fmadd_pd(__mv_vop651, __mv_vop652, __mv_accm213);	// latency = 5
  // stmt vectorized: y[226] += A[555] * x[225]
__mv_hi = _mm_shuffle_pd(__mv_accm213, __mv_accm213, 0x1);
__mv_accm213 = _mm_add_pd(__mv_accm213, __mv_hi);
y[226] = y[226]+__mv_accm213[0];;
  // stmt vectorized: y[226] += A[556] * x[227];
  __mv_accm214 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop654 = _mm256_maskload_pd(&A[((i0 + 0) + 557)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop655 = _mm256_maskload_pd(&x[((i0 + 0) + 226)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm214 = _mm256_fmadd_pd(__mv_vop654, __mv_vop655, __mv_accm214);	// latency = 5
    // stmt vectorized: y[0 * i0 + 227] += A[1 * i0 + 557] * x[1 * i0 + 226];
  }
  __mv_vop657 = _mm_loadu_pd(&A[560]);	// latency = 1
  __mv_vop658 = _mm_i64gather_pd(&x[227], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm215 = _mm_fmadd_pd(__mv_vop657, __mv_vop658, __mv_accm215);	// latency = 5
  // stmt vectorized: y[228] += A[560] * x[227]
__mv_hi = _mm_shuffle_pd(__mv_accm215, __mv_accm215, 0x1);
__mv_accm215 = _mm_add_pd(__mv_accm215, __mv_hi);
y[228] = y[228]+__mv_accm215[0];;
  // stmt vectorized: y[228] += A[561] * x[229];
  __mv_accm216 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop660 = _mm256_maskload_pd(&A[((i0 + 0) + 562)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop661 = _mm256_maskload_pd(&x[((i0 + 0) + 228)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm216 = _mm256_fmadd_pd(__mv_vop660, __mv_vop661, __mv_accm216);	// latency = 5
    // stmt vectorized: y[0 * i0 + 229] += A[1 * i0 + 562] * x[1 * i0 + 228];
  }__mv_accm210 = _mm256_hadd_pd(__mv_accm210, __mv_accm212);	// latency = 6
__mv_accm210 = _mm256_add_pd(_mm256_blend_pd(__mv_accm210,__mv_accm214,0b1100), _mm256_permute2f128_pd(__mv_accm210,__mv_accm214,0x21));	// latency = 4
y[223] = y[223] + __mv_accm210[0];	// latency = 1
y[225] = y[225] + __mv_accm210[1];	// latency = 1
y[227] = y[227] + __mv_accm210[2];	// latency = 1
y[229] = y[229] + __mv_accm210[3];	// latency = 1

  __mv_vop663 = _mm_loadu_pd(&A[565]);	// latency = 1
  __mv_vop664 = _mm_i64gather_pd(&x[229], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm217 = _mm_fmadd_pd(__mv_vop663, __mv_vop664, __mv_accm217);	// latency = 5
  // stmt vectorized: y[230] += A[565] * x[229]
__mv_hi = _mm_shuffle_pd(__mv_accm217, __mv_accm217, 0x1);
__mv_accm217 = _mm_add_pd(__mv_accm217, __mv_hi);
y[230] = y[230]+__mv_accm217[0];;
  // stmt vectorized: y[230] += A[566] * x[231];
  __mv_accm218 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop666 = _mm256_maskload_pd(&A[((i0 + 0) + 567)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop667 = _mm256_maskload_pd(&x[((i0 + 0) + 230)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm218 = _mm256_fmadd_pd(__mv_vop666, __mv_vop667, __mv_accm218);	// latency = 5
    // stmt vectorized: y[0 * i0 + 231] += A[1 * i0 + 567] * x[1 * i0 + 230];
  }
  __mv_vop669 = _mm_loadu_pd(&A[570]);	// latency = 1
  __mv_vop670 = _mm_i64gather_pd(&x[231], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm219 = _mm_fmadd_pd(__mv_vop669, __mv_vop670, __mv_accm219);	// latency = 5
  // stmt vectorized: y[232] += A[570] * x[231]
__mv_hi = _mm_shuffle_pd(__mv_accm219, __mv_accm219, 0x1);
__mv_accm219 = _mm_add_pd(__mv_accm219, __mv_hi);
y[232] = y[232]+__mv_accm219[0];;
  // stmt vectorized: y[232] += A[571] * x[233];
  __mv_accm220 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop672 = _mm256_maskload_pd(&A[((i0 + 0) + 572)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop673 = _mm256_maskload_pd(&x[((i0 + 0) + 232)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm220 = _mm256_fmadd_pd(__mv_vop672, __mv_vop673, __mv_accm220);	// latency = 5
    // stmt vectorized: y[0 * i0 + 233] += A[1 * i0 + 572] * x[1 * i0 + 232];
  }
  __mv_vop675 = _mm_loadu_pd(&A[575]);	// latency = 1
  __mv_vop676 = _mm_i64gather_pd(&x[233], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm221 = _mm_fmadd_pd(__mv_vop675, __mv_vop676, __mv_accm221);	// latency = 5
  // stmt vectorized: y[234] += A[575] * x[233]
__mv_hi = _mm_shuffle_pd(__mv_accm221, __mv_accm221, 0x1);
__mv_accm221 = _mm_add_pd(__mv_accm221, __mv_hi);
y[234] = y[234]+__mv_accm221[0];;
  // stmt vectorized: y[234] += A[576] * x[235];
  __mv_accm222 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop678 = _mm256_maskload_pd(&A[((i0 + 0) + 577)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop679 = _mm256_maskload_pd(&x[((i0 + 0) + 234)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm222 = _mm256_fmadd_pd(__mv_vop678, __mv_vop679, __mv_accm222);	// latency = 5
    // stmt vectorized: y[0 * i0 + 235] += A[1 * i0 + 577] * x[1 * i0 + 234];
  }
  __mv_vop681 = _mm_loadu_pd(&A[580]);	// latency = 1
  __mv_vop682 = _mm_i64gather_pd(&x[235], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm223 = _mm_fmadd_pd(__mv_vop681, __mv_vop682, __mv_accm223);	// latency = 5
  // stmt vectorized: y[236] += A[580] * x[235]
__mv_hi = _mm_shuffle_pd(__mv_accm223, __mv_accm223, 0x1);
__mv_accm223 = _mm_add_pd(__mv_accm223, __mv_hi);
y[236] = y[236]+__mv_accm223[0];;
  // stmt vectorized: y[236] += A[581] * x[237];
  __mv_accm224 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop684 = _mm256_maskload_pd(&A[((i0 + 0) + 582)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop685 = _mm256_maskload_pd(&x[((i0 + 0) + 236)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm224 = _mm256_fmadd_pd(__mv_vop684, __mv_vop685, __mv_accm224);	// latency = 5
    // stmt vectorized: y[0 * i0 + 237] += A[1 * i0 + 582] * x[1 * i0 + 236];
  }__mv_accm218 = _mm256_hadd_pd(__mv_accm218, __mv_accm220);	// latency = 6
__mv_accm218 = _mm256_add_pd(_mm256_blend_pd(__mv_accm218,__mv_accm222,0b1100), _mm256_permute2f128_pd(__mv_accm218,__mv_accm222,0x21));	// latency = 4
y[231] = y[231] + __mv_accm218[0];	// latency = 1
y[233] = y[233] + __mv_accm218[1];	// latency = 1
y[235] = y[235] + __mv_accm218[2];	// latency = 1
y[237] = y[237] + __mv_accm218[3];	// latency = 1

  __mv_vop687 = _mm_loadu_pd(&A[585]);	// latency = 1
  __mv_vop688 = _mm_i64gather_pd(&x[237], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm225 = _mm_fmadd_pd(__mv_vop687, __mv_vop688, __mv_accm225);	// latency = 5
  // stmt vectorized: y[238] += A[585] * x[237]
__mv_hi = _mm_shuffle_pd(__mv_accm225, __mv_accm225, 0x1);
__mv_accm225 = _mm_add_pd(__mv_accm225, __mv_hi);
y[238] = y[238]+__mv_accm225[0];;
  // stmt vectorized: y[238] += A[586] * x[239];
  __mv_accm226 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop690 = _mm256_maskload_pd(&A[((i0 + 0) + 587)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop691 = _mm256_maskload_pd(&x[((i0 + 0) + 238)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm226 = _mm256_fmadd_pd(__mv_vop690, __mv_vop691, __mv_accm226);	// latency = 5
    // stmt vectorized: y[0 * i0 + 239] += A[1 * i0 + 587] * x[1 * i0 + 238];
  }
  __mv_vop693 = _mm_loadu_pd(&A[590]);	// latency = 1
  __mv_vop694 = _mm_i64gather_pd(&x[239], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm227 = _mm_fmadd_pd(__mv_vop693, __mv_vop694, __mv_accm227);	// latency = 5
  // stmt vectorized: y[240] += A[590] * x[239]
__mv_hi = _mm_shuffle_pd(__mv_accm227, __mv_accm227, 0x1);
__mv_accm227 = _mm_add_pd(__mv_accm227, __mv_hi);
y[240] = y[240]+__mv_accm227[0];;
  // stmt vectorized: y[240] += A[591] * x[241];
  __mv_accm228 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop696 = _mm256_maskload_pd(&A[((i0 + 0) + 592)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop697 = _mm256_maskload_pd(&x[((i0 + 0) + 240)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm228 = _mm256_fmadd_pd(__mv_vop696, __mv_vop697, __mv_accm228);	// latency = 5
    // stmt vectorized: y[0 * i0 + 241] += A[1 * i0 + 592] * x[1 * i0 + 240];
  }
  __mv_vop699 = _mm_loadu_pd(&A[595]);	// latency = 1
  __mv_vop700 = _mm_i64gather_pd(&x[241], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm229 = _mm_fmadd_pd(__mv_vop699, __mv_vop700, __mv_accm229);	// latency = 5
  // stmt vectorized: y[242] += A[595] * x[241]
__mv_hi = _mm_shuffle_pd(__mv_accm229, __mv_accm229, 0x1);
__mv_accm229 = _mm_add_pd(__mv_accm229, __mv_hi);
y[242] = y[242]+__mv_accm229[0];;
  // stmt vectorized: y[242] += A[596] * x[243];
  __mv_accm230 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop702 = _mm256_maskload_pd(&A[((i0 + 0) + 597)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop703 = _mm256_maskload_pd(&x[((i0 + 0) + 242)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm230 = _mm256_fmadd_pd(__mv_vop702, __mv_vop703, __mv_accm230);	// latency = 5
    // stmt vectorized: y[0 * i0 + 243] += A[1 * i0 + 597] * x[1 * i0 + 242];
  }
  __mv_vop705 = _mm_loadu_pd(&A[600]);	// latency = 1
  __mv_vop706 = _mm_i64gather_pd(&x[243], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm231 = _mm_fmadd_pd(__mv_vop705, __mv_vop706, __mv_accm231);	// latency = 5
  // stmt vectorized: y[244] += A[600] * x[243]
__mv_hi = _mm_shuffle_pd(__mv_accm231, __mv_accm231, 0x1);
__mv_accm231 = _mm_add_pd(__mv_accm231, __mv_hi);
y[244] = y[244]+__mv_accm231[0];;
  // stmt vectorized: y[244] += A[601] * x[245];
  __mv_accm232 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop708 = _mm256_maskload_pd(&A[((i0 + 0) + 602)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop709 = _mm256_maskload_pd(&x[((i0 + 0) + 244)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm232 = _mm256_fmadd_pd(__mv_vop708, __mv_vop709, __mv_accm232);	// latency = 5
    // stmt vectorized: y[0 * i0 + 245] += A[1 * i0 + 602] * x[1 * i0 + 244];
  }__mv_accm226 = _mm256_hadd_pd(__mv_accm226, __mv_accm228);	// latency = 6
__mv_accm226 = _mm256_add_pd(_mm256_blend_pd(__mv_accm226,__mv_accm230,0b1100), _mm256_permute2f128_pd(__mv_accm226,__mv_accm230,0x21));	// latency = 4
y[239] = y[239] + __mv_accm226[0];	// latency = 1
y[241] = y[241] + __mv_accm226[1];	// latency = 1
y[243] = y[243] + __mv_accm226[2];	// latency = 1
y[245] = y[245] + __mv_accm226[3];	// latency = 1

  __mv_vop711 = _mm_loadu_pd(&A[605]);	// latency = 1
  __mv_vop712 = _mm_i64gather_pd(&x[245], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm233 = _mm_fmadd_pd(__mv_vop711, __mv_vop712, __mv_accm233);	// latency = 5
  // stmt vectorized: y[246] += A[605] * x[245]
__mv_hi = _mm_shuffle_pd(__mv_accm233, __mv_accm233, 0x1);
__mv_accm233 = _mm_add_pd(__mv_accm233, __mv_hi);
y[246] = y[246]+__mv_accm233[0];;
  // stmt vectorized: y[246] += A[606] * x[247];
  __mv_accm234 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop714 = _mm256_maskload_pd(&A[((i0 + 0) + 607)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop715 = _mm256_maskload_pd(&x[((i0 + 0) + 246)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm234 = _mm256_fmadd_pd(__mv_vop714, __mv_vop715, __mv_accm234);	// latency = 5
    // stmt vectorized: y[0 * i0 + 247] += A[1 * i0 + 607] * x[1 * i0 + 246];
  }
  __mv_vop717 = _mm_loadu_pd(&A[610]);	// latency = 1
  __mv_vop718 = _mm_i64gather_pd(&x[247], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm235 = _mm_fmadd_pd(__mv_vop717, __mv_vop718, __mv_accm235);	// latency = 5
  // stmt vectorized: y[248] += A[610] * x[247]
__mv_hi = _mm_shuffle_pd(__mv_accm235, __mv_accm235, 0x1);
__mv_accm235 = _mm_add_pd(__mv_accm235, __mv_hi);
y[248] = y[248]+__mv_accm235[0];;
  // stmt vectorized: y[248] += A[611] * x[249];
  __mv_accm236 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop720 = _mm256_maskload_pd(&A[((i0 + 0) + 612)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop721 = _mm256_maskload_pd(&x[((i0 + 0) + 248)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm236 = _mm256_fmadd_pd(__mv_vop720, __mv_vop721, __mv_accm236);	// latency = 5
    // stmt vectorized: y[0 * i0 + 249] += A[1 * i0 + 612] * x[1 * i0 + 248];
  }
  __mv_vop723 = _mm_loadu_pd(&A[615]);	// latency = 1
  __mv_vop724 = _mm_i64gather_pd(&x[249], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm237 = _mm_fmadd_pd(__mv_vop723, __mv_vop724, __mv_accm237);	// latency = 5
  // stmt vectorized: y[250] += A[615] * x[249]
__mv_hi = _mm_shuffle_pd(__mv_accm237, __mv_accm237, 0x1);
__mv_accm237 = _mm_add_pd(__mv_accm237, __mv_hi);
y[250] = y[250]+__mv_accm237[0];;
  // stmt vectorized: y[250] += A[616] * x[251];
  __mv_accm238 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop726 = _mm256_maskload_pd(&A[((i0 + 0) + 617)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop727 = _mm256_maskload_pd(&x[((i0 + 0) + 250)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm238 = _mm256_fmadd_pd(__mv_vop726, __mv_vop727, __mv_accm238);	// latency = 5
    // stmt vectorized: y[0 * i0 + 251] += A[1 * i0 + 617] * x[1 * i0 + 250];
  }
  __mv_vop729 = _mm_loadu_pd(&A[620]);	// latency = 1
  __mv_vop730 = _mm_i64gather_pd(&x[251], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm239 = _mm_fmadd_pd(__mv_vop729, __mv_vop730, __mv_accm239);	// latency = 5
  // stmt vectorized: y[252] += A[620] * x[251]
__mv_hi = _mm_shuffle_pd(__mv_accm239, __mv_accm239, 0x1);
__mv_accm239 = _mm_add_pd(__mv_accm239, __mv_hi);
y[252] = y[252]+__mv_accm239[0];;
  // stmt vectorized: y[252] += A[621] * x[253];
  __mv_accm240 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop732 = _mm256_maskload_pd(&A[((i0 + 0) + 622)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop733 = _mm256_maskload_pd(&x[((i0 + 0) + 252)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm240 = _mm256_fmadd_pd(__mv_vop732, __mv_vop733, __mv_accm240);	// latency = 5
    // stmt vectorized: y[0 * i0 + 253] += A[1 * i0 + 622] * x[1 * i0 + 252];
  }__mv_accm234 = _mm256_hadd_pd(__mv_accm234, __mv_accm236);	// latency = 6
__mv_accm234 = _mm256_add_pd(_mm256_blend_pd(__mv_accm234,__mv_accm238,0b1100), _mm256_permute2f128_pd(__mv_accm234,__mv_accm238,0x21));	// latency = 4
y[247] = y[247] + __mv_accm234[0];	// latency = 1
y[249] = y[249] + __mv_accm234[1];	// latency = 1
y[251] = y[251] + __mv_accm234[2];	// latency = 1
y[253] = y[253] + __mv_accm234[3];	// latency = 1

  __mv_vop735 = _mm_loadu_pd(&A[625]);	// latency = 1
  __mv_vop736 = _mm_i64gather_pd(&x[253], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm241 = _mm_fmadd_pd(__mv_vop735, __mv_vop736, __mv_accm241);	// latency = 5
  // stmt vectorized: y[254] += A[625] * x[253]
__mv_hi = _mm_shuffle_pd(__mv_accm241, __mv_accm241, 0x1);
__mv_accm241 = _mm_add_pd(__mv_accm241, __mv_hi);
y[254] = y[254]+__mv_accm241[0];;
  // stmt vectorized: y[254] += A[626] * x[255];
  __mv_accm242 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop738 = _mm256_maskload_pd(&A[((i0 + 0) + 627)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop739 = _mm256_maskload_pd(&x[((i0 + 0) + 254)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm242 = _mm256_fmadd_pd(__mv_vop738, __mv_vop739, __mv_accm242);	// latency = 5
    // stmt vectorized: y[0 * i0 + 255] += A[1 * i0 + 627] * x[1 * i0 + 254];
  }
  __mv_vop741 = _mm_loadu_pd(&A[630]);	// latency = 1
  __mv_vop742 = _mm_i64gather_pd(&x[255], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm243 = _mm_fmadd_pd(__mv_vop741, __mv_vop742, __mv_accm243);	// latency = 5
  // stmt vectorized: y[256] += A[630] * x[255]
__mv_hi = _mm_shuffle_pd(__mv_accm243, __mv_accm243, 0x1);
__mv_accm243 = _mm_add_pd(__mv_accm243, __mv_hi);
y[256] = y[256]+__mv_accm243[0];;
  // stmt vectorized: y[256] += A[631] * x[257];
  __mv_accm244 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop744 = _mm256_maskload_pd(&A[((i0 + 0) + 632)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop745 = _mm256_maskload_pd(&x[((i0 + 0) + 256)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm244 = _mm256_fmadd_pd(__mv_vop744, __mv_vop745, __mv_accm244);	// latency = 5
    // stmt vectorized: y[0 * i0 + 257] += A[1 * i0 + 632] * x[1 * i0 + 256];
  }
  __mv_vop747 = _mm_loadu_pd(&A[635]);	// latency = 1
  __mv_vop748 = _mm_i64gather_pd(&x[257], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm245 = _mm_fmadd_pd(__mv_vop747, __mv_vop748, __mv_accm245);	// latency = 5
  // stmt vectorized: y[258] += A[635] * x[257]
__mv_hi = _mm_shuffle_pd(__mv_accm245, __mv_accm245, 0x1);
__mv_accm245 = _mm_add_pd(__mv_accm245, __mv_hi);
y[258] = y[258]+__mv_accm245[0];;
  // stmt vectorized: y[258] += A[636] * x[259];
  __mv_accm246 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop750 = _mm256_maskload_pd(&A[((i0 + 0) + 637)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop751 = _mm256_maskload_pd(&x[((i0 + 0) + 258)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm246 = _mm256_fmadd_pd(__mv_vop750, __mv_vop751, __mv_accm246);	// latency = 5
    // stmt vectorized: y[0 * i0 + 259] += A[1 * i0 + 637] * x[1 * i0 + 258];
  }
  __mv_vop753 = _mm_loadu_pd(&A[640]);	// latency = 1
  __mv_vop754 = _mm_i64gather_pd(&x[259], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm247 = _mm_fmadd_pd(__mv_vop753, __mv_vop754, __mv_accm247);	// latency = 5
  // stmt vectorized: y[260] += A[640] * x[259]
__mv_hi = _mm_shuffle_pd(__mv_accm247, __mv_accm247, 0x1);
__mv_accm247 = _mm_add_pd(__mv_accm247, __mv_hi);
y[260] = y[260]+__mv_accm247[0];;
  // stmt vectorized: y[260] += A[641] * x[261];
  __mv_accm248 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop756 = _mm256_maskload_pd(&A[((i0 + 0) + 642)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop757 = _mm256_maskload_pd(&x[((i0 + 0) + 260)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm248 = _mm256_fmadd_pd(__mv_vop756, __mv_vop757, __mv_accm248);	// latency = 5
    // stmt vectorized: y[0 * i0 + 261] += A[1 * i0 + 642] * x[1 * i0 + 260];
  }__mv_accm242 = _mm256_hadd_pd(__mv_accm242, __mv_accm244);	// latency = 6
__mv_accm242 = _mm256_add_pd(_mm256_blend_pd(__mv_accm242,__mv_accm246,0b1100), _mm256_permute2f128_pd(__mv_accm242,__mv_accm246,0x21));	// latency = 4
y[255] = y[255] + __mv_accm242[0];	// latency = 1
y[257] = y[257] + __mv_accm242[1];	// latency = 1
y[259] = y[259] + __mv_accm242[2];	// latency = 1
y[261] = y[261] + __mv_accm242[3];	// latency = 1

  __mv_vop759 = _mm_loadu_pd(&A[645]);	// latency = 1
  __mv_vop760 = _mm_i64gather_pd(&x[261], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm249 = _mm_fmadd_pd(__mv_vop759, __mv_vop760, __mv_accm249);	// latency = 5
  // stmt vectorized: y[262] += A[645] * x[261]
__mv_hi = _mm_shuffle_pd(__mv_accm249, __mv_accm249, 0x1);
__mv_accm249 = _mm_add_pd(__mv_accm249, __mv_hi);
y[262] = y[262]+__mv_accm249[0];;
  // stmt vectorized: y[262] += A[646] * x[263];
  __mv_accm250 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop762 = _mm256_maskload_pd(&A[((i0 + 0) + 647)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop763 = _mm256_maskload_pd(&x[((i0 + 0) + 262)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm250 = _mm256_fmadd_pd(__mv_vop762, __mv_vop763, __mv_accm250);	// latency = 5
    // stmt vectorized: y[0 * i0 + 263] += A[1 * i0 + 647] * x[1 * i0 + 262];
  }
  __mv_vop765 = _mm_loadu_pd(&A[650]);	// latency = 1
  __mv_vop766 = _mm_i64gather_pd(&x[263], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm251 = _mm_fmadd_pd(__mv_vop765, __mv_vop766, __mv_accm251);	// latency = 5
  // stmt vectorized: y[264] += A[650] * x[263]
__mv_hi = _mm_shuffle_pd(__mv_accm251, __mv_accm251, 0x1);
__mv_accm251 = _mm_add_pd(__mv_accm251, __mv_hi);
y[264] = y[264]+__mv_accm251[0];;
  // stmt vectorized: y[264] += A[651] * x[265];
  __mv_accm252 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop768 = _mm256_maskload_pd(&A[((i0 + 0) + 652)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop769 = _mm256_maskload_pd(&x[((i0 + 0) + 264)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm252 = _mm256_fmadd_pd(__mv_vop768, __mv_vop769, __mv_accm252);	// latency = 5
    // stmt vectorized: y[0 * i0 + 265] += A[1 * i0 + 652] * x[1 * i0 + 264];
  }
  __mv_vop771 = _mm_loadu_pd(&A[655]);	// latency = 1
  __mv_vop772 = _mm_i64gather_pd(&x[265], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm253 = _mm_fmadd_pd(__mv_vop771, __mv_vop772, __mv_accm253);	// latency = 5
  // stmt vectorized: y[266] += A[655] * x[265]
__mv_hi = _mm_shuffle_pd(__mv_accm253, __mv_accm253, 0x1);
__mv_accm253 = _mm_add_pd(__mv_accm253, __mv_hi);
y[266] = y[266]+__mv_accm253[0];;
  // stmt vectorized: y[266] += A[656] * x[267];
  __mv_accm254 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop774 = _mm256_maskload_pd(&A[((i0 + 0) + 657)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop775 = _mm256_maskload_pd(&x[((i0 + 0) + 266)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm254 = _mm256_fmadd_pd(__mv_vop774, __mv_vop775, __mv_accm254);	// latency = 5
    // stmt vectorized: y[0 * i0 + 267] += A[1 * i0 + 657] * x[1 * i0 + 266];
  }
  __mv_vop777 = _mm_loadu_pd(&A[660]);	// latency = 1
  __mv_vop778 = _mm_i64gather_pd(&x[267], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm255 = _mm_fmadd_pd(__mv_vop777, __mv_vop778, __mv_accm255);	// latency = 5
  // stmt vectorized: y[268] += A[660] * x[267]
__mv_hi = _mm_shuffle_pd(__mv_accm255, __mv_accm255, 0x1);
__mv_accm255 = _mm_add_pd(__mv_accm255, __mv_hi);
y[268] = y[268]+__mv_accm255[0];;
  // stmt vectorized: y[268] += A[661] * x[269];
  __mv_accm256 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop780 = _mm256_maskload_pd(&A[((i0 + 0) + 662)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop781 = _mm256_maskload_pd(&x[((i0 + 0) + 268)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm256 = _mm256_fmadd_pd(__mv_vop780, __mv_vop781, __mv_accm256);	// latency = 5
    // stmt vectorized: y[0 * i0 + 269] += A[1 * i0 + 662] * x[1 * i0 + 268];
  }__mv_accm250 = _mm256_hadd_pd(__mv_accm250, __mv_accm252);	// latency = 6
__mv_accm250 = _mm256_add_pd(_mm256_blend_pd(__mv_accm250,__mv_accm254,0b1100), _mm256_permute2f128_pd(__mv_accm250,__mv_accm254,0x21));	// latency = 4
y[263] = y[263] + __mv_accm250[0];	// latency = 1
y[265] = y[265] + __mv_accm250[1];	// latency = 1
y[267] = y[267] + __mv_accm250[2];	// latency = 1
y[269] = y[269] + __mv_accm250[3];	// latency = 1

  __mv_vop783 = _mm_loadu_pd(&A[665]);	// latency = 1
  __mv_vop784 = _mm_i64gather_pd(&x[269], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm257 = _mm_fmadd_pd(__mv_vop783, __mv_vop784, __mv_accm257);	// latency = 5
  // stmt vectorized: y[270] += A[665] * x[269]
__mv_hi = _mm_shuffle_pd(__mv_accm257, __mv_accm257, 0x1);
__mv_accm257 = _mm_add_pd(__mv_accm257, __mv_hi);
y[270] = y[270]+__mv_accm257[0];;
  // stmt vectorized: y[270] += A[666] * x[271];
  __mv_accm258 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop786 = _mm256_maskload_pd(&A[((i0 + 0) + 667)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop787 = _mm256_maskload_pd(&x[((i0 + 0) + 270)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm258 = _mm256_fmadd_pd(__mv_vop786, __mv_vop787, __mv_accm258);	// latency = 5
    // stmt vectorized: y[0 * i0 + 271] += A[1 * i0 + 667] * x[1 * i0 + 270];
  }
  __mv_vop789 = _mm_loadu_pd(&A[670]);	// latency = 1
  __mv_vop790 = _mm_i64gather_pd(&x[271], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm259 = _mm_fmadd_pd(__mv_vop789, __mv_vop790, __mv_accm259);	// latency = 5
  // stmt vectorized: y[272] += A[670] * x[271]
__mv_hi = _mm_shuffle_pd(__mv_accm259, __mv_accm259, 0x1);
__mv_accm259 = _mm_add_pd(__mv_accm259, __mv_hi);
y[272] = y[272]+__mv_accm259[0];;
  // stmt vectorized: y[272] += A[671] * x[273];
  __mv_accm260 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop792 = _mm256_maskload_pd(&A[((i0 + 0) + 672)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop793 = _mm256_maskload_pd(&x[((i0 + 0) + 272)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm260 = _mm256_fmadd_pd(__mv_vop792, __mv_vop793, __mv_accm260);	// latency = 5
    // stmt vectorized: y[0 * i0 + 273] += A[1 * i0 + 672] * x[1 * i0 + 272];
  }
  __mv_vop795 = _mm_loadu_pd(&A[675]);	// latency = 1
  __mv_vop796 = _mm_i64gather_pd(&x[273], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm261 = _mm_fmadd_pd(__mv_vop795, __mv_vop796, __mv_accm261);	// latency = 5
  // stmt vectorized: y[274] += A[675] * x[273]
__mv_hi = _mm_shuffle_pd(__mv_accm261, __mv_accm261, 0x1);
__mv_accm261 = _mm_add_pd(__mv_accm261, __mv_hi);
y[274] = y[274]+__mv_accm261[0];;
  // stmt vectorized: y[274] += A[676] * x[275];
  __mv_accm262 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop798 = _mm256_maskload_pd(&A[((i0 + 0) + 677)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop799 = _mm256_maskload_pd(&x[((i0 + 0) + 274)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm262 = _mm256_fmadd_pd(__mv_vop798, __mv_vop799, __mv_accm262);	// latency = 5
    // stmt vectorized: y[0 * i0 + 275] += A[1 * i0 + 677] * x[1 * i0 + 274];
  }
  __mv_vop801 = _mm_loadu_pd(&A[680]);	// latency = 1
  __mv_vop802 = _mm_i64gather_pd(&x[275], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm263 = _mm_fmadd_pd(__mv_vop801, __mv_vop802, __mv_accm263);	// latency = 5
  // stmt vectorized: y[276] += A[680] * x[275]
__mv_hi = _mm_shuffle_pd(__mv_accm263, __mv_accm263, 0x1);
__mv_accm263 = _mm_add_pd(__mv_accm263, __mv_hi);
y[276] = y[276]+__mv_accm263[0];;
  // stmt vectorized: y[276] += A[681] * x[277];
  __mv_accm264 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop804 = _mm256_maskload_pd(&A[((i0 + 0) + 682)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop805 = _mm256_maskload_pd(&x[((i0 + 0) + 276)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm264 = _mm256_fmadd_pd(__mv_vop804, __mv_vop805, __mv_accm264);	// latency = 5
    // stmt vectorized: y[0 * i0 + 277] += A[1 * i0 + 682] * x[1 * i0 + 276];
  }__mv_accm258 = _mm256_hadd_pd(__mv_accm258, __mv_accm260);	// latency = 6
__mv_accm258 = _mm256_add_pd(_mm256_blend_pd(__mv_accm258,__mv_accm262,0b1100), _mm256_permute2f128_pd(__mv_accm258,__mv_accm262,0x21));	// latency = 4
y[271] = y[271] + __mv_accm258[0];	// latency = 1
y[273] = y[273] + __mv_accm258[1];	// latency = 1
y[275] = y[275] + __mv_accm258[2];	// latency = 1
y[277] = y[277] + __mv_accm258[3];	// latency = 1

  __mv_vop807 = _mm_loadu_pd(&A[685]);	// latency = 1
  __mv_vop808 = _mm_i64gather_pd(&x[277], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm265 = _mm_fmadd_pd(__mv_vop807, __mv_vop808, __mv_accm265);	// latency = 5
  // stmt vectorized: y[278] += A[685] * x[277]
__mv_hi = _mm_shuffle_pd(__mv_accm265, __mv_accm265, 0x1);
__mv_accm265 = _mm_add_pd(__mv_accm265, __mv_hi);
y[278] = y[278]+__mv_accm265[0];;
  // stmt vectorized: y[278] += A[686] * x[279];
  __mv_accm266 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop810 = _mm256_maskload_pd(&A[((i0 + 0) + 687)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop811 = _mm256_maskload_pd(&x[((i0 + 0) + 278)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm266 = _mm256_fmadd_pd(__mv_vop810, __mv_vop811, __mv_accm266);	// latency = 5
    // stmt vectorized: y[0 * i0 + 279] += A[1 * i0 + 687] * x[1 * i0 + 278];
  }
  __mv_vop813 = _mm_loadu_pd(&A[690]);	// latency = 1
  __mv_vop814 = _mm_i64gather_pd(&x[279], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm267 = _mm_fmadd_pd(__mv_vop813, __mv_vop814, __mv_accm267);	// latency = 5
  // stmt vectorized: y[280] += A[690] * x[279]
__mv_hi = _mm_shuffle_pd(__mv_accm267, __mv_accm267, 0x1);
__mv_accm267 = _mm_add_pd(__mv_accm267, __mv_hi);
y[280] = y[280]+__mv_accm267[0];;
  // stmt vectorized: y[280] += A[691] * x[281];
  __mv_accm268 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop816 = _mm256_maskload_pd(&A[((i0 + 0) + 692)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop817 = _mm256_maskload_pd(&x[((i0 + 0) + 280)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm268 = _mm256_fmadd_pd(__mv_vop816, __mv_vop817, __mv_accm268);	// latency = 5
    // stmt vectorized: y[0 * i0 + 281] += A[1 * i0 + 692] * x[1 * i0 + 280];
  }
  __mv_vop819 = _mm_loadu_pd(&A[695]);	// latency = 1
  __mv_vop820 = _mm_i64gather_pd(&x[281], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm269 = _mm_fmadd_pd(__mv_vop819, __mv_vop820, __mv_accm269);	// latency = 5
  // stmt vectorized: y[282] += A[695] * x[281]
__mv_hi = _mm_shuffle_pd(__mv_accm269, __mv_accm269, 0x1);
__mv_accm269 = _mm_add_pd(__mv_accm269, __mv_hi);
y[282] = y[282]+__mv_accm269[0];;
  // stmt vectorized: y[282] += A[696] * x[283];
  __mv_accm270 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop822 = _mm256_maskload_pd(&A[((i0 + 0) + 697)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop823 = _mm256_maskload_pd(&x[((i0 + 0) + 282)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm270 = _mm256_fmadd_pd(__mv_vop822, __mv_vop823, __mv_accm270);	// latency = 5
    // stmt vectorized: y[0 * i0 + 283] += A[1 * i0 + 697] * x[1 * i0 + 282];
  }
  __mv_vop825 = _mm_loadu_pd(&A[700]);	// latency = 1
  __mv_vop826 = _mm_i64gather_pd(&x[283], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm271 = _mm_fmadd_pd(__mv_vop825, __mv_vop826, __mv_accm271);	// latency = 5
  // stmt vectorized: y[284] += A[700] * x[283]
__mv_hi = _mm_shuffle_pd(__mv_accm271, __mv_accm271, 0x1);
__mv_accm271 = _mm_add_pd(__mv_accm271, __mv_hi);
y[284] = y[284]+__mv_accm271[0];;
  // stmt vectorized: y[284] += A[701] * x[285];
  __mv_accm272 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop828 = _mm256_maskload_pd(&A[((i0 + 0) + 702)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop829 = _mm256_maskload_pd(&x[((i0 + 0) + 284)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm272 = _mm256_fmadd_pd(__mv_vop828, __mv_vop829, __mv_accm272);	// latency = 5
    // stmt vectorized: y[0 * i0 + 285] += A[1 * i0 + 702] * x[1 * i0 + 284];
  }__mv_accm266 = _mm256_hadd_pd(__mv_accm266, __mv_accm268);	// latency = 6
__mv_accm266 = _mm256_add_pd(_mm256_blend_pd(__mv_accm266,__mv_accm270,0b1100), _mm256_permute2f128_pd(__mv_accm266,__mv_accm270,0x21));	// latency = 4
y[279] = y[279] + __mv_accm266[0];	// latency = 1
y[281] = y[281] + __mv_accm266[1];	// latency = 1
y[283] = y[283] + __mv_accm266[2];	// latency = 1
y[285] = y[285] + __mv_accm266[3];	// latency = 1

  __mv_vop831 = _mm_loadu_pd(&A[705]);	// latency = 1
  __mv_vop832 = _mm_i64gather_pd(&x[285], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm273 = _mm_fmadd_pd(__mv_vop831, __mv_vop832, __mv_accm273);	// latency = 5
  // stmt vectorized: y[286] += A[705] * x[285]
__mv_hi = _mm_shuffle_pd(__mv_accm273, __mv_accm273, 0x1);
__mv_accm273 = _mm_add_pd(__mv_accm273, __mv_hi);
y[286] = y[286]+__mv_accm273[0];;
  // stmt vectorized: y[286] += A[706] * x[287];
  __mv_accm274 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop834 = _mm256_maskload_pd(&A[((i0 + 0) + 707)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop835 = _mm256_maskload_pd(&x[((i0 + 0) + 286)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm274 = _mm256_fmadd_pd(__mv_vop834, __mv_vop835, __mv_accm274);	// latency = 5
    // stmt vectorized: y[0 * i0 + 287] += A[1 * i0 + 707] * x[1 * i0 + 286];
  }
  __mv_vop837 = _mm_loadu_pd(&A[710]);	// latency = 1
  __mv_vop838 = _mm_i64gather_pd(&x[287], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm275 = _mm_fmadd_pd(__mv_vop837, __mv_vop838, __mv_accm275);	// latency = 5
  // stmt vectorized: y[288] += A[710] * x[287]
__mv_hi = _mm_shuffle_pd(__mv_accm275, __mv_accm275, 0x1);
__mv_accm275 = _mm_add_pd(__mv_accm275, __mv_hi);
y[288] = y[288]+__mv_accm275[0];;
  // stmt vectorized: y[288] += A[711] * x[289];
  __mv_accm276 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop840 = _mm256_maskload_pd(&A[((i0 + 0) + 712)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop841 = _mm256_maskload_pd(&x[((i0 + 0) + 288)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm276 = _mm256_fmadd_pd(__mv_vop840, __mv_vop841, __mv_accm276);	// latency = 5
    // stmt vectorized: y[0 * i0 + 289] += A[1 * i0 + 712] * x[1 * i0 + 288];
  }
  __mv_vop843 = _mm_loadu_pd(&A[715]);	// latency = 1
  __mv_vop844 = _mm_i64gather_pd(&x[289], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm277 = _mm_fmadd_pd(__mv_vop843, __mv_vop844, __mv_accm277);	// latency = 5
  // stmt vectorized: y[290] += A[715] * x[289]
__mv_hi = _mm_shuffle_pd(__mv_accm277, __mv_accm277, 0x1);
__mv_accm277 = _mm_add_pd(__mv_accm277, __mv_hi);
y[290] = y[290]+__mv_accm277[0];;
  // stmt vectorized: y[290] += A[716] * x[291];
  __mv_accm278 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop846 = _mm256_maskload_pd(&A[((i0 + 0) + 717)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop847 = _mm256_maskload_pd(&x[((i0 + 0) + 290)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm278 = _mm256_fmadd_pd(__mv_vop846, __mv_vop847, __mv_accm278);	// latency = 5
    // stmt vectorized: y[0 * i0 + 291] += A[1 * i0 + 717] * x[1 * i0 + 290];
  }
  __mv_vop849 = _mm_loadu_pd(&A[720]);	// latency = 1
  __mv_vop850 = _mm_i64gather_pd(&x[291], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm279 = _mm_fmadd_pd(__mv_vop849, __mv_vop850, __mv_accm279);	// latency = 5
  // stmt vectorized: y[292] += A[720] * x[291]
__mv_hi = _mm_shuffle_pd(__mv_accm279, __mv_accm279, 0x1);
__mv_accm279 = _mm_add_pd(__mv_accm279, __mv_hi);
y[292] = y[292]+__mv_accm279[0];;
  // stmt vectorized: y[292] += A[721] * x[293];
  __mv_accm280 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop852 = _mm256_maskload_pd(&A[((i0 + 0) + 722)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop853 = _mm256_maskload_pd(&x[((i0 + 0) + 292)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm280 = _mm256_fmadd_pd(__mv_vop852, __mv_vop853, __mv_accm280);	// latency = 5
    // stmt vectorized: y[0 * i0 + 293] += A[1 * i0 + 722] * x[1 * i0 + 292];
  }__mv_accm274 = _mm256_hadd_pd(__mv_accm274, __mv_accm276);	// latency = 6
__mv_accm274 = _mm256_add_pd(_mm256_blend_pd(__mv_accm274,__mv_accm278,0b1100), _mm256_permute2f128_pd(__mv_accm274,__mv_accm278,0x21));	// latency = 4
y[287] = y[287] + __mv_accm274[0];	// latency = 1
y[289] = y[289] + __mv_accm274[1];	// latency = 1
y[291] = y[291] + __mv_accm274[2];	// latency = 1
y[293] = y[293] + __mv_accm274[3];	// latency = 1

  __mv_vop855 = _mm_loadu_pd(&A[725]);	// latency = 1
  __mv_vop856 = _mm_i64gather_pd(&x[293], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm281 = _mm_fmadd_pd(__mv_vop855, __mv_vop856, __mv_accm281);	// latency = 5
  // stmt vectorized: y[294] += A[725] * x[293]
__mv_hi = _mm_shuffle_pd(__mv_accm281, __mv_accm281, 0x1);
__mv_accm281 = _mm_add_pd(__mv_accm281, __mv_hi);
y[294] = y[294]+__mv_accm281[0];;
  // stmt vectorized: y[294] += A[726] * x[295];
  __mv_accm282 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop858 = _mm256_maskload_pd(&A[((i0 + 0) + 727)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop859 = _mm256_maskload_pd(&x[((i0 + 0) + 294)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm282 = _mm256_fmadd_pd(__mv_vop858, __mv_vop859, __mv_accm282);	// latency = 5
    // stmt vectorized: y[0 * i0 + 295] += A[1 * i0 + 727] * x[1 * i0 + 294];
  }
  __mv_vop861 = _mm_loadu_pd(&A[730]);	// latency = 1
  __mv_vop862 = _mm_i64gather_pd(&x[295], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm283 = _mm_fmadd_pd(__mv_vop861, __mv_vop862, __mv_accm283);	// latency = 5
  // stmt vectorized: y[296] += A[730] * x[295]
__mv_hi = _mm_shuffle_pd(__mv_accm283, __mv_accm283, 0x1);
__mv_accm283 = _mm_add_pd(__mv_accm283, __mv_hi);
y[296] = y[296]+__mv_accm283[0];;
  // stmt vectorized: y[296] += A[731] * x[297];
  __mv_accm284 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop864 = _mm256_maskload_pd(&A[((i0 + 0) + 732)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop865 = _mm256_maskload_pd(&x[((i0 + 0) + 296)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm284 = _mm256_fmadd_pd(__mv_vop864, __mv_vop865, __mv_accm284);	// latency = 5
    // stmt vectorized: y[0 * i0 + 297] += A[1 * i0 + 732] * x[1 * i0 + 296];
  }
  __mv_vop867 = _mm_loadu_pd(&A[735]);	// latency = 1
  __mv_vop868 = _mm_i64gather_pd(&x[297], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm285 = _mm_fmadd_pd(__mv_vop867, __mv_vop868, __mv_accm285);	// latency = 5
  // stmt vectorized: y[298] += A[735] * x[297]
__mv_hi = _mm_shuffle_pd(__mv_accm285, __mv_accm285, 0x1);
__mv_accm285 = _mm_add_pd(__mv_accm285, __mv_hi);
y[298] = y[298]+__mv_accm285[0];;
  // stmt vectorized: y[298] += A[736] * x[299];
  __mv_accm286 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop870 = _mm256_maskload_pd(&A[((i0 + 0) + 737)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop871 = _mm256_maskload_pd(&x[((i0 + 0) + 298)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm286 = _mm256_fmadd_pd(__mv_vop870, __mv_vop871, __mv_accm286);	// latency = 5
    // stmt vectorized: y[0 * i0 + 299] += A[1 * i0 + 737] * x[1 * i0 + 298];
  }
  __mv_vop873 = _mm_loadu_pd(&A[740]);	// latency = 1
  __mv_vop874 = _mm_i64gather_pd(&x[299], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm287 = _mm_fmadd_pd(__mv_vop873, __mv_vop874, __mv_accm287);	// latency = 5
  // stmt vectorized: y[300] += A[740] * x[299]
__mv_hi = _mm_shuffle_pd(__mv_accm287, __mv_accm287, 0x1);
__mv_accm287 = _mm_add_pd(__mv_accm287, __mv_hi);
y[300] = y[300]+__mv_accm287[0];;
  // stmt vectorized: y[300] += A[741] * x[301];
  __mv_accm288 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop876 = _mm256_maskload_pd(&A[((i0 + 0) + 742)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop877 = _mm256_maskload_pd(&x[((i0 + 0) + 300)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm288 = _mm256_fmadd_pd(__mv_vop876, __mv_vop877, __mv_accm288);	// latency = 5
    // stmt vectorized: y[0 * i0 + 301] += A[1 * i0 + 742] * x[1 * i0 + 300];
  }__mv_accm282 = _mm256_hadd_pd(__mv_accm282, __mv_accm284);	// latency = 6
__mv_accm282 = _mm256_add_pd(_mm256_blend_pd(__mv_accm282,__mv_accm286,0b1100), _mm256_permute2f128_pd(__mv_accm282,__mv_accm286,0x21));	// latency = 4
y[295] = y[295] + __mv_accm282[0];	// latency = 1
y[297] = y[297] + __mv_accm282[1];	// latency = 1
y[299] = y[299] + __mv_accm282[2];	// latency = 1
y[301] = y[301] + __mv_accm282[3];	// latency = 1

  __mv_vop879 = _mm_loadu_pd(&A[745]);	// latency = 1
  __mv_vop880 = _mm_i64gather_pd(&x[301], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm289 = _mm_fmadd_pd(__mv_vop879, __mv_vop880, __mv_accm289);	// latency = 5
  // stmt vectorized: y[302] += A[745] * x[301]
__mv_hi = _mm_shuffle_pd(__mv_accm289, __mv_accm289, 0x1);
__mv_accm289 = _mm_add_pd(__mv_accm289, __mv_hi);
y[302] = y[302]+__mv_accm289[0];;
  // stmt vectorized: y[302] += A[746] * x[303];
  __mv_accm290 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop882 = _mm256_maskload_pd(&A[((i0 + 0) + 747)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop883 = _mm256_maskload_pd(&x[((i0 + 0) + 302)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm290 = _mm256_fmadd_pd(__mv_vop882, __mv_vop883, __mv_accm290);	// latency = 5
    // stmt vectorized: y[0 * i0 + 303] += A[1 * i0 + 747] * x[1 * i0 + 302];
  }
  __mv_vop885 = _mm_loadu_pd(&A[750]);	// latency = 1
  __mv_vop886 = _mm_i64gather_pd(&x[303], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm291 = _mm_fmadd_pd(__mv_vop885, __mv_vop886, __mv_accm291);	// latency = 5
  // stmt vectorized: y[304] += A[750] * x[303]
__mv_hi = _mm_shuffle_pd(__mv_accm291, __mv_accm291, 0x1);
__mv_accm291 = _mm_add_pd(__mv_accm291, __mv_hi);
y[304] = y[304]+__mv_accm291[0];;
  // stmt vectorized: y[304] += A[751] * x[305];
  __mv_accm292 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop888 = _mm256_maskload_pd(&A[((i0 + 0) + 752)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop889 = _mm256_maskload_pd(&x[((i0 + 0) + 304)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm292 = _mm256_fmadd_pd(__mv_vop888, __mv_vop889, __mv_accm292);	// latency = 5
    // stmt vectorized: y[0 * i0 + 305] += A[1 * i0 + 752] * x[1 * i0 + 304];
  }
  __mv_vop891 = _mm_loadu_pd(&A[755]);	// latency = 1
  __mv_vop892 = _mm_i64gather_pd(&x[305], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm293 = _mm_fmadd_pd(__mv_vop891, __mv_vop892, __mv_accm293);	// latency = 5
  // stmt vectorized: y[306] += A[755] * x[305]
__mv_hi = _mm_shuffle_pd(__mv_accm293, __mv_accm293, 0x1);
__mv_accm293 = _mm_add_pd(__mv_accm293, __mv_hi);
y[306] = y[306]+__mv_accm293[0];;
  // stmt vectorized: y[306] += A[756] * x[307];
  __mv_accm294 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop894 = _mm256_maskload_pd(&A[((i0 + 0) + 757)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop895 = _mm256_maskload_pd(&x[((i0 + 0) + 306)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm294 = _mm256_fmadd_pd(__mv_vop894, __mv_vop895, __mv_accm294);	// latency = 5
    // stmt vectorized: y[0 * i0 + 307] += A[1 * i0 + 757] * x[1 * i0 + 306];
  }
  __mv_vop897 = _mm_loadu_pd(&A[760]);	// latency = 1
  __mv_vop898 = _mm_i64gather_pd(&x[307], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm295 = _mm_fmadd_pd(__mv_vop897, __mv_vop898, __mv_accm295);	// latency = 5
  // stmt vectorized: y[308] += A[760] * x[307]
__mv_hi = _mm_shuffle_pd(__mv_accm295, __mv_accm295, 0x1);
__mv_accm295 = _mm_add_pd(__mv_accm295, __mv_hi);
y[308] = y[308]+__mv_accm295[0];;
  // stmt vectorized: y[308] += A[761] * x[309];
  __mv_accm296 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop900 = _mm256_maskload_pd(&A[((i0 + 0) + 762)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop901 = _mm256_maskload_pd(&x[((i0 + 0) + 308)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm296 = _mm256_fmadd_pd(__mv_vop900, __mv_vop901, __mv_accm296);	// latency = 5
    // stmt vectorized: y[0 * i0 + 309] += A[1 * i0 + 762] * x[1 * i0 + 308];
  }__mv_accm290 = _mm256_hadd_pd(__mv_accm290, __mv_accm292);	// latency = 6
__mv_accm290 = _mm256_add_pd(_mm256_blend_pd(__mv_accm290,__mv_accm294,0b1100), _mm256_permute2f128_pd(__mv_accm290,__mv_accm294,0x21));	// latency = 4
y[303] = y[303] + __mv_accm290[0];	// latency = 1
y[305] = y[305] + __mv_accm290[1];	// latency = 1
y[307] = y[307] + __mv_accm290[2];	// latency = 1
y[309] = y[309] + __mv_accm290[3];	// latency = 1

  __mv_vop903 = _mm_loadu_pd(&A[765]);	// latency = 1
  __mv_vop904 = _mm_i64gather_pd(&x[309], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm297 = _mm_fmadd_pd(__mv_vop903, __mv_vop904, __mv_accm297);	// latency = 5
  // stmt vectorized: y[310] += A[765] * x[309]
__mv_hi = _mm_shuffle_pd(__mv_accm297, __mv_accm297, 0x1);
__mv_accm297 = _mm_add_pd(__mv_accm297, __mv_hi);
y[310] = y[310]+__mv_accm297[0];;
  // stmt vectorized: y[310] += A[766] * x[311];
  __mv_accm298 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop906 = _mm256_maskload_pd(&A[((i0 + 0) + 767)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop907 = _mm256_maskload_pd(&x[((i0 + 0) + 310)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm298 = _mm256_fmadd_pd(__mv_vop906, __mv_vop907, __mv_accm298);	// latency = 5
    // stmt vectorized: y[0 * i0 + 311] += A[1 * i0 + 767] * x[1 * i0 + 310];
  }
  __mv_vop909 = _mm_loadu_pd(&A[770]);	// latency = 1
  __mv_vop910 = _mm_i64gather_pd(&x[311], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm299 = _mm_fmadd_pd(__mv_vop909, __mv_vop910, __mv_accm299);	// latency = 5
  // stmt vectorized: y[312] += A[770] * x[311]
__mv_hi = _mm_shuffle_pd(__mv_accm299, __mv_accm299, 0x1);
__mv_accm299 = _mm_add_pd(__mv_accm299, __mv_hi);
y[312] = y[312]+__mv_accm299[0];;
  // stmt vectorized: y[312] += A[771] * x[313];
  __mv_accm300 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop912 = _mm256_maskload_pd(&A[((i0 + 0) + 772)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop913 = _mm256_maskload_pd(&x[((i0 + 0) + 312)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm300 = _mm256_fmadd_pd(__mv_vop912, __mv_vop913, __mv_accm300);	// latency = 5
    // stmt vectorized: y[0 * i0 + 313] += A[1 * i0 + 772] * x[1 * i0 + 312];
  }
  __mv_vop915 = _mm_loadu_pd(&A[775]);	// latency = 1
  __mv_vop916 = _mm_i64gather_pd(&x[313], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm301 = _mm_fmadd_pd(__mv_vop915, __mv_vop916, __mv_accm301);	// latency = 5
  // stmt vectorized: y[314] += A[775] * x[313]
__mv_hi = _mm_shuffle_pd(__mv_accm301, __mv_accm301, 0x1);
__mv_accm301 = _mm_add_pd(__mv_accm301, __mv_hi);
y[314] = y[314]+__mv_accm301[0];;
  // stmt vectorized: y[314] += A[776] * x[315];
  __mv_accm302 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop918 = _mm256_maskload_pd(&A[((i0 + 0) + 777)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop919 = _mm256_maskload_pd(&x[((i0 + 0) + 314)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm302 = _mm256_fmadd_pd(__mv_vop918, __mv_vop919, __mv_accm302);	// latency = 5
    // stmt vectorized: y[0 * i0 + 315] += A[1 * i0 + 777] * x[1 * i0 + 314];
  }
  __mv_vop921 = _mm_loadu_pd(&A[780]);	// latency = 1
  __mv_vop922 = _mm_i64gather_pd(&x[315], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm303 = _mm_fmadd_pd(__mv_vop921, __mv_vop922, __mv_accm303);	// latency = 5
  // stmt vectorized: y[316] += A[780] * x[315]
__mv_hi = _mm_shuffle_pd(__mv_accm303, __mv_accm303, 0x1);
__mv_accm303 = _mm_add_pd(__mv_accm303, __mv_hi);
y[316] = y[316]+__mv_accm303[0];;
  // stmt vectorized: y[316] += A[781] * x[317];
  __mv_accm304 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop924 = _mm256_maskload_pd(&A[((i0 + 0) + 782)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop925 = _mm256_maskload_pd(&x[((i0 + 0) + 316)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm304 = _mm256_fmadd_pd(__mv_vop924, __mv_vop925, __mv_accm304);	// latency = 5
    // stmt vectorized: y[0 * i0 + 317] += A[1 * i0 + 782] * x[1 * i0 + 316];
  }__mv_accm298 = _mm256_hadd_pd(__mv_accm298, __mv_accm300);	// latency = 6
__mv_accm298 = _mm256_add_pd(_mm256_blend_pd(__mv_accm298,__mv_accm302,0b1100), _mm256_permute2f128_pd(__mv_accm298,__mv_accm302,0x21));	// latency = 4
y[311] = y[311] + __mv_accm298[0];	// latency = 1
y[313] = y[313] + __mv_accm298[1];	// latency = 1
y[315] = y[315] + __mv_accm298[2];	// latency = 1
y[317] = y[317] + __mv_accm298[3];	// latency = 1

  __mv_vop927 = _mm_loadu_pd(&A[785]);	// latency = 1
  __mv_vop928 = _mm_i64gather_pd(&x[317], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm305 = _mm_fmadd_pd(__mv_vop927, __mv_vop928, __mv_accm305);	// latency = 5
  // stmt vectorized: y[318] += A[785] * x[317]
__mv_hi = _mm_shuffle_pd(__mv_accm305, __mv_accm305, 0x1);
__mv_accm305 = _mm_add_pd(__mv_accm305, __mv_hi);
y[318] = y[318]+__mv_accm305[0];;
  // stmt vectorized: y[318] += A[786] * x[319];
  __mv_accm306 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop930 = _mm256_maskload_pd(&A[((i0 + 0) + 787)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop931 = _mm256_maskload_pd(&x[((i0 + 0) + 318)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm306 = _mm256_fmadd_pd(__mv_vop930, __mv_vop931, __mv_accm306);	// latency = 5
    // stmt vectorized: y[0 * i0 + 319] += A[1 * i0 + 787] * x[1 * i0 + 318];
  }
  __mv_vop933 = _mm_loadu_pd(&A[790]);	// latency = 1
  __mv_vop934 = _mm_i64gather_pd(&x[319], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm307 = _mm_fmadd_pd(__mv_vop933, __mv_vop934, __mv_accm307);	// latency = 5
  // stmt vectorized: y[320] += A[790] * x[319]
__mv_hi = _mm_shuffle_pd(__mv_accm307, __mv_accm307, 0x1);
__mv_accm307 = _mm_add_pd(__mv_accm307, __mv_hi);
y[320] = y[320]+__mv_accm307[0];;
  // stmt vectorized: y[320] += A[791] * x[321];
  __mv_accm308 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop936 = _mm256_maskload_pd(&A[((i0 + 0) + 792)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop937 = _mm256_maskload_pd(&x[((i0 + 0) + 320)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm308 = _mm256_fmadd_pd(__mv_vop936, __mv_vop937, __mv_accm308);	// latency = 5
    // stmt vectorized: y[0 * i0 + 321] += A[1 * i0 + 792] * x[1 * i0 + 320];
  }
  __mv_vop939 = _mm_loadu_pd(&A[795]);	// latency = 1
  __mv_vop940 = _mm_i64gather_pd(&x[321], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm309 = _mm_fmadd_pd(__mv_vop939, __mv_vop940, __mv_accm309);	// latency = 5
  // stmt vectorized: y[322] += A[795] * x[321]
__mv_hi = _mm_shuffle_pd(__mv_accm309, __mv_accm309, 0x1);
__mv_accm309 = _mm_add_pd(__mv_accm309, __mv_hi);
y[322] = y[322]+__mv_accm309[0];;
  // stmt vectorized: y[322] += A[796] * x[323];
  __mv_accm310 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop942 = _mm256_maskload_pd(&A[((i0 + 0) + 797)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop943 = _mm256_maskload_pd(&x[((i0 + 0) + 322)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm310 = _mm256_fmadd_pd(__mv_vop942, __mv_vop943, __mv_accm310);	// latency = 5
    // stmt vectorized: y[0 * i0 + 323] += A[1 * i0 + 797] * x[1 * i0 + 322];
  }
  __mv_vop945 = _mm_loadu_pd(&A[800]);	// latency = 1
  __mv_vop946 = _mm_i64gather_pd(&x[323], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm311 = _mm_fmadd_pd(__mv_vop945, __mv_vop946, __mv_accm311);	// latency = 5
  // stmt vectorized: y[324] += A[800] * x[323]
__mv_hi = _mm_shuffle_pd(__mv_accm311, __mv_accm311, 0x1);
__mv_accm311 = _mm_add_pd(__mv_accm311, __mv_hi);
y[324] = y[324]+__mv_accm311[0];;
  // stmt vectorized: y[324] += A[801] * x[325];
  __mv_accm312 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop948 = _mm256_maskload_pd(&A[((i0 + 0) + 802)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop949 = _mm256_maskload_pd(&x[((i0 + 0) + 324)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm312 = _mm256_fmadd_pd(__mv_vop948, __mv_vop949, __mv_accm312);	// latency = 5
    // stmt vectorized: y[0 * i0 + 325] += A[1 * i0 + 802] * x[1 * i0 + 324];
  }__mv_accm306 = _mm256_hadd_pd(__mv_accm306, __mv_accm308);	// latency = 6
__mv_accm306 = _mm256_add_pd(_mm256_blend_pd(__mv_accm306,__mv_accm310,0b1100), _mm256_permute2f128_pd(__mv_accm306,__mv_accm310,0x21));	// latency = 4
y[319] = y[319] + __mv_accm306[0];	// latency = 1
y[321] = y[321] + __mv_accm306[1];	// latency = 1
y[323] = y[323] + __mv_accm306[2];	// latency = 1
y[325] = y[325] + __mv_accm306[3];	// latency = 1

  __mv_vop951 = _mm_loadu_pd(&A[805]);	// latency = 1
  __mv_vop952 = _mm_i64gather_pd(&x[325], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm313 = _mm_fmadd_pd(__mv_vop951, __mv_vop952, __mv_accm313);	// latency = 5
  // stmt vectorized: y[326] += A[805] * x[325]
__mv_hi = _mm_shuffle_pd(__mv_accm313, __mv_accm313, 0x1);
__mv_accm313 = _mm_add_pd(__mv_accm313, __mv_hi);
y[326] = y[326]+__mv_accm313[0];;
  // stmt vectorized: y[326] += A[806] * x[327];
  __mv_accm314 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop954 = _mm256_maskload_pd(&A[((i0 + 0) + 807)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop955 = _mm256_maskload_pd(&x[((i0 + 0) + 326)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm314 = _mm256_fmadd_pd(__mv_vop954, __mv_vop955, __mv_accm314);	// latency = 5
    // stmt vectorized: y[0 * i0 + 327] += A[1 * i0 + 807] * x[1 * i0 + 326];
  }
  __mv_vop957 = _mm_loadu_pd(&A[810]);	// latency = 1
  __mv_vop958 = _mm_i64gather_pd(&x[327], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm315 = _mm_fmadd_pd(__mv_vop957, __mv_vop958, __mv_accm315);	// latency = 5
  // stmt vectorized: y[328] += A[810] * x[327]
__mv_hi = _mm_shuffle_pd(__mv_accm315, __mv_accm315, 0x1);
__mv_accm315 = _mm_add_pd(__mv_accm315, __mv_hi);
y[328] = y[328]+__mv_accm315[0];;
  // stmt vectorized: y[328] += A[811] * x[329];
  __mv_accm316 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop960 = _mm256_maskload_pd(&A[((i0 + 0) + 812)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop961 = _mm256_maskload_pd(&x[((i0 + 0) + 328)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm316 = _mm256_fmadd_pd(__mv_vop960, __mv_vop961, __mv_accm316);	// latency = 5
    // stmt vectorized: y[0 * i0 + 329] += A[1 * i0 + 812] * x[1 * i0 + 328];
  }
  __mv_vop963 = _mm_loadu_pd(&A[815]);	// latency = 1
  __mv_vop964 = _mm_i64gather_pd(&x[329], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm317 = _mm_fmadd_pd(__mv_vop963, __mv_vop964, __mv_accm317);	// latency = 5
  // stmt vectorized: y[330] += A[815] * x[329]
__mv_hi = _mm_shuffle_pd(__mv_accm317, __mv_accm317, 0x1);
__mv_accm317 = _mm_add_pd(__mv_accm317, __mv_hi);
y[330] = y[330]+__mv_accm317[0];;
  // stmt vectorized: y[330] += A[816] * x[331];
  __mv_accm318 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop966 = _mm256_maskload_pd(&A[((i0 + 0) + 817)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop967 = _mm256_maskload_pd(&x[((i0 + 0) + 330)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm318 = _mm256_fmadd_pd(__mv_vop966, __mv_vop967, __mv_accm318);	// latency = 5
    // stmt vectorized: y[0 * i0 + 331] += A[1 * i0 + 817] * x[1 * i0 + 330];
  }
  __mv_vop969 = _mm_loadu_pd(&A[820]);	// latency = 1
  __mv_vop970 = _mm_i64gather_pd(&x[331], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm319 = _mm_fmadd_pd(__mv_vop969, __mv_vop970, __mv_accm319);	// latency = 5
  // stmt vectorized: y[332] += A[820] * x[331]
__mv_hi = _mm_shuffle_pd(__mv_accm319, __mv_accm319, 0x1);
__mv_accm319 = _mm_add_pd(__mv_accm319, __mv_hi);
y[332] = y[332]+__mv_accm319[0];;
  // stmt vectorized: y[332] += A[821] * x[333];
  __mv_accm320 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop972 = _mm256_maskload_pd(&A[((i0 + 0) + 822)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop973 = _mm256_maskload_pd(&x[((i0 + 0) + 332)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm320 = _mm256_fmadd_pd(__mv_vop972, __mv_vop973, __mv_accm320);	// latency = 5
    // stmt vectorized: y[0 * i0 + 333] += A[1 * i0 + 822] * x[1 * i0 + 332];
  }__mv_accm314 = _mm256_hadd_pd(__mv_accm314, __mv_accm316);	// latency = 6
__mv_accm314 = _mm256_add_pd(_mm256_blend_pd(__mv_accm314,__mv_accm318,0b1100), _mm256_permute2f128_pd(__mv_accm314,__mv_accm318,0x21));	// latency = 4
y[327] = y[327] + __mv_accm314[0];	// latency = 1
y[329] = y[329] + __mv_accm314[1];	// latency = 1
y[331] = y[331] + __mv_accm314[2];	// latency = 1
y[333] = y[333] + __mv_accm314[3];	// latency = 1

  __mv_vop975 = _mm256_loadu_pd(&A[825]);	// latency = 1
  __mv_vop976 = _mm256_i64gather_pd(&x[2], _mm256_set_epi64x(4018, 4012, 331, 0), 8);	// latency = 10
  __mv_accm321 = _mm256_fmadd_pd(__mv_vop975, __mv_vop976, __mv_accm321);	// latency = 5
  // stmt vectorized: y[334] += A[825] * x[2]
__mv_lo256 = _mm256_castpd256_pd128(__mv_accm321);
__mv_hi256 = _mm256_extractf128_pd(__mv_accm321, 0x1);
__mv_lo256 = _mm_add_pd(__mv_lo256, __mv_hi256);
__mv_hi256 = _mm_shuffle_pd(__mv_lo256, __mv_lo256, 0x1);
__mv_lo256 = _mm_add_pd(__mv_lo256, __mv_hi256);
y[334] = y[334]+__mv_lo256[0];;
  // stmt vectorized: y[334] += A[826] * x[333];
  // stmt vectorized: y[334] += A[827] * x[4014];
  // stmt vectorized: y[334] += A[828] * x[4020];
  __mv_vop978 = _mm_loadu_pd(&A[829]);	// latency = 1
  __mv_vop979 = _mm_i64gather_pd(&x[336], _mm_set_epi64x(3678, 0), 8);	// latency = 10
  __mv_accm322 = _mm_fmadd_pd(__mv_vop978, __mv_vop979, __mv_accm322);	// latency = 5
  // stmt vectorized: y[335] += A[829] * x[336]
__mv_hi = _mm_shuffle_pd(__mv_accm322, __mv_accm322, 0x1);
__mv_accm322 = _mm_add_pd(__mv_accm322, __mv_hi);
y[335] = y[335]+__mv_accm322[0];;
  // stmt vectorized: y[335] += A[830] * x[4014];
  __mv_accm323 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop981 = _mm256_maskload_pd(&A[((i0 + 0) + 831)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop982 = _mm256_maskload_pd(&x[((i0 + 0) + 335)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm323 = _mm256_fmadd_pd(__mv_vop981, __mv_vop982, __mv_accm323);	// latency = 5
    // stmt vectorized: y[0 * i0 + 336] += A[1 * i0 + 831] * x[1 * i0 + 335];
  }
  __mv_vop984 = _mm_loadu_pd(&A[834]);	// latency = 1
  __mv_vop985 = _mm_i64gather_pd(&x[336], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm324 = _mm_fmadd_pd(__mv_vop984, __mv_vop985, __mv_accm324);	// latency = 5
  // stmt vectorized: y[337] += A[834] * x[336]
__mv_hi = _mm_shuffle_pd(__mv_accm324, __mv_accm324, 0x1);
__mv_accm324 = _mm_add_pd(__mv_accm324, __mv_hi);
y[337] = y[337]+__mv_accm324[0];;
  // stmt vectorized: y[337] += A[835] * x[338];
  __mv_accm325 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop987 = _mm256_maskload_pd(&A[((i0 + 0) + 836)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop988 = _mm256_maskload_pd(&x[((i0 + 0) + 337)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm325 = _mm256_fmadd_pd(__mv_vop987, __mv_vop988, __mv_accm325);	// latency = 5
    // stmt vectorized: y[0 * i0 + 338] += A[1 * i0 + 836] * x[1 * i0 + 337];
  }
  __mv_vop990 = _mm_loadu_pd(&A[839]);	// latency = 1
  __mv_vop991 = _mm_i64gather_pd(&x[338], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm326 = _mm_fmadd_pd(__mv_vop990, __mv_vop991, __mv_accm326);	// latency = 5
  // stmt vectorized: y[339] += A[839] * x[338]
__mv_hi = _mm_shuffle_pd(__mv_accm326, __mv_accm326, 0x1);
__mv_accm326 = _mm_add_pd(__mv_accm326, __mv_hi);
y[339] = y[339]+__mv_accm326[0];;
  // stmt vectorized: y[339] += A[840] * x[340];
  __mv_accm327 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop993 = _mm256_maskload_pd(&A[((i0 + 0) + 841)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop994 = _mm256_maskload_pd(&x[((i0 + 0) + 339)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm327 = _mm256_fmadd_pd(__mv_vop993, __mv_vop994, __mv_accm327);	// latency = 5
    // stmt vectorized: y[0 * i0 + 340] += A[1 * i0 + 841] * x[1 * i0 + 339];
  }
  __mv_vop996 = _mm_loadu_pd(&A[844]);	// latency = 1
  __mv_vop997 = _mm_i64gather_pd(&x[340], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm328 = _mm_fmadd_pd(__mv_vop996, __mv_vop997, __mv_accm328);	// latency = 5
  // stmt vectorized: y[341] += A[844] * x[340]
__mv_hi = _mm_shuffle_pd(__mv_accm328, __mv_accm328, 0x1);
__mv_accm328 = _mm_add_pd(__mv_accm328, __mv_hi);
y[341] = y[341]+__mv_accm328[0];;
  // stmt vectorized: y[341] += A[845] * x[342];
  __mv_accm329 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop999 = _mm256_maskload_pd(&A[((i0 + 0) + 846)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1000 = _mm256_maskload_pd(&x[((i0 + 0) + 341)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm329 = _mm256_fmadd_pd(__mv_vop999, __mv_vop1000, __mv_accm329);	// latency = 5
    // stmt vectorized: y[0 * i0 + 342] += A[1 * i0 + 846] * x[1 * i0 + 341];
  }__mv_accm323 = _mm256_hadd_pd(__mv_accm323, __mv_accm325);	// latency = 6
__mv_accm323 = _mm256_add_pd(_mm256_blend_pd(__mv_accm323,__mv_accm327,0b1100), _mm256_permute2f128_pd(__mv_accm323,__mv_accm327,0x21));	// latency = 4
y[336] = y[336] + __mv_accm323[0];	// latency = 1
y[338] = y[338] + __mv_accm323[1];	// latency = 1
y[340] = y[340] + __mv_accm323[2];	// latency = 1
y[342] = y[342] + __mv_accm323[3];	// latency = 1

  __mv_vop1002 = _mm_loadu_pd(&A[849]);	// latency = 1
  __mv_vop1003 = _mm_i64gather_pd(&x[342], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm330 = _mm_fmadd_pd(__mv_vop1002, __mv_vop1003, __mv_accm330);	// latency = 5
  // stmt vectorized: y[343] += A[849] * x[342]
__mv_hi = _mm_shuffle_pd(__mv_accm330, __mv_accm330, 0x1);
__mv_accm330 = _mm_add_pd(__mv_accm330, __mv_hi);
y[343] = y[343]+__mv_accm330[0];;
  // stmt vectorized: y[343] += A[850] * x[344];
  __mv_accm331 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1005 = _mm256_maskload_pd(&A[((i0 + 0) + 851)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1006 = _mm256_maskload_pd(&x[((i0 + 0) + 343)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm331 = _mm256_fmadd_pd(__mv_vop1005, __mv_vop1006, __mv_accm331);	// latency = 5
    // stmt vectorized: y[0 * i0 + 344] += A[1 * i0 + 851] * x[1 * i0 + 343];
  }
  __mv_vop1008 = _mm_loadu_pd(&A[854]);	// latency = 1
  __mv_vop1009 = _mm_i64gather_pd(&x[344], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm332 = _mm_fmadd_pd(__mv_vop1008, __mv_vop1009, __mv_accm332);	// latency = 5
  // stmt vectorized: y[345] += A[854] * x[344]
__mv_hi = _mm_shuffle_pd(__mv_accm332, __mv_accm332, 0x1);
__mv_accm332 = _mm_add_pd(__mv_accm332, __mv_hi);
y[345] = y[345]+__mv_accm332[0];;
  // stmt vectorized: y[345] += A[855] * x[346];
  __mv_accm333 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1011 = _mm256_maskload_pd(&A[((i0 + 0) + 856)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1012 = _mm256_maskload_pd(&x[((i0 + 0) + 345)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm333 = _mm256_fmadd_pd(__mv_vop1011, __mv_vop1012, __mv_accm333);	// latency = 5
    // stmt vectorized: y[0 * i0 + 346] += A[1 * i0 + 856] * x[1 * i0 + 345];
  }
  __mv_vop1014 = _mm_loadu_pd(&A[859]);	// latency = 1
  __mv_vop1015 = _mm_i64gather_pd(&x[346], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm334 = _mm_fmadd_pd(__mv_vop1014, __mv_vop1015, __mv_accm334);	// latency = 5
  // stmt vectorized: y[347] += A[859] * x[346]
__mv_hi = _mm_shuffle_pd(__mv_accm334, __mv_accm334, 0x1);
__mv_accm334 = _mm_add_pd(__mv_accm334, __mv_hi);
y[347] = y[347]+__mv_accm334[0];;
  // stmt vectorized: y[347] += A[860] * x[348];
  __mv_accm335 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1017 = _mm256_maskload_pd(&A[((i0 + 0) + 861)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1018 = _mm256_maskload_pd(&x[((i0 + 0) + 347)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm335 = _mm256_fmadd_pd(__mv_vop1017, __mv_vop1018, __mv_accm335);	// latency = 5
    // stmt vectorized: y[0 * i0 + 348] += A[1 * i0 + 861] * x[1 * i0 + 347];
  }
  __mv_vop1020 = _mm_loadu_pd(&A[864]);	// latency = 1
  __mv_vop1021 = _mm_i64gather_pd(&x[348], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm336 = _mm_fmadd_pd(__mv_vop1020, __mv_vop1021, __mv_accm336);	// latency = 5
  // stmt vectorized: y[349] += A[864] * x[348]
__mv_hi = _mm_shuffle_pd(__mv_accm336, __mv_accm336, 0x1);
__mv_accm336 = _mm_add_pd(__mv_accm336, __mv_hi);
y[349] = y[349]+__mv_accm336[0];;
  // stmt vectorized: y[349] += A[865] * x[350];
  __mv_accm337 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1023 = _mm256_maskload_pd(&A[((i0 + 0) + 866)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1024 = _mm256_maskload_pd(&x[((i0 + 0) + 349)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm337 = _mm256_fmadd_pd(__mv_vop1023, __mv_vop1024, __mv_accm337);	// latency = 5
    // stmt vectorized: y[0 * i0 + 350] += A[1 * i0 + 866] * x[1 * i0 + 349];
  }__mv_accm331 = _mm256_hadd_pd(__mv_accm331, __mv_accm333);	// latency = 6
__mv_accm331 = _mm256_add_pd(_mm256_blend_pd(__mv_accm331,__mv_accm335,0b1100), _mm256_permute2f128_pd(__mv_accm331,__mv_accm335,0x21));	// latency = 4
y[344] = y[344] + __mv_accm331[0];	// latency = 1
y[346] = y[346] + __mv_accm331[1];	// latency = 1
y[348] = y[348] + __mv_accm331[2];	// latency = 1
y[350] = y[350] + __mv_accm331[3];	// latency = 1

  __mv_vop1026 = _mm_loadu_pd(&A[869]);	// latency = 1
  __mv_vop1027 = _mm_i64gather_pd(&x[350], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm338 = _mm_fmadd_pd(__mv_vop1026, __mv_vop1027, __mv_accm338);	// latency = 5
  // stmt vectorized: y[351] += A[869] * x[350]
__mv_hi = _mm_shuffle_pd(__mv_accm338, __mv_accm338, 0x1);
__mv_accm338 = _mm_add_pd(__mv_accm338, __mv_hi);
y[351] = y[351]+__mv_accm338[0];;
  // stmt vectorized: y[351] += A[870] * x[352];
  __mv_accm339 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1029 = _mm256_maskload_pd(&A[((i0 + 0) + 871)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1030 = _mm256_maskload_pd(&x[((i0 + 0) + 351)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm339 = _mm256_fmadd_pd(__mv_vop1029, __mv_vop1030, __mv_accm339);	// latency = 5
    // stmt vectorized: y[0 * i0 + 352] += A[1 * i0 + 871] * x[1 * i0 + 351];
  }
  __mv_vop1032 = _mm_loadu_pd(&A[874]);	// latency = 1
  __mv_vop1033 = _mm_i64gather_pd(&x[352], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm340 = _mm_fmadd_pd(__mv_vop1032, __mv_vop1033, __mv_accm340);	// latency = 5
  // stmt vectorized: y[353] += A[874] * x[352]
__mv_hi = _mm_shuffle_pd(__mv_accm340, __mv_accm340, 0x1);
__mv_accm340 = _mm_add_pd(__mv_accm340, __mv_hi);
y[353] = y[353]+__mv_accm340[0];;
  // stmt vectorized: y[353] += A[875] * x[354];
  __mv_accm341 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1035 = _mm256_maskload_pd(&A[((i0 + 0) + 876)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1036 = _mm256_maskload_pd(&x[((i0 + 0) + 353)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm341 = _mm256_fmadd_pd(__mv_vop1035, __mv_vop1036, __mv_accm341);	// latency = 5
    // stmt vectorized: y[0 * i0 + 354] += A[1 * i0 + 876] * x[1 * i0 + 353];
  }
  __mv_vop1038 = _mm_loadu_pd(&A[879]);	// latency = 1
  __mv_vop1039 = _mm_i64gather_pd(&x[354], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm342 = _mm_fmadd_pd(__mv_vop1038, __mv_vop1039, __mv_accm342);	// latency = 5
  // stmt vectorized: y[355] += A[879] * x[354]
__mv_hi = _mm_shuffle_pd(__mv_accm342, __mv_accm342, 0x1);
__mv_accm342 = _mm_add_pd(__mv_accm342, __mv_hi);
y[355] = y[355]+__mv_accm342[0];;
  // stmt vectorized: y[355] += A[880] * x[356];
  __mv_accm343 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1041 = _mm256_maskload_pd(&A[((i0 + 0) + 881)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1042 = _mm256_maskload_pd(&x[((i0 + 0) + 355)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm343 = _mm256_fmadd_pd(__mv_vop1041, __mv_vop1042, __mv_accm343);	// latency = 5
    // stmt vectorized: y[0 * i0 + 356] += A[1 * i0 + 881] * x[1 * i0 + 355];
  }
  __mv_vop1044 = _mm_loadu_pd(&A[884]);	// latency = 1
  __mv_vop1045 = _mm_i64gather_pd(&x[356], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm344 = _mm_fmadd_pd(__mv_vop1044, __mv_vop1045, __mv_accm344);	// latency = 5
  // stmt vectorized: y[357] += A[884] * x[356]
__mv_hi = _mm_shuffle_pd(__mv_accm344, __mv_accm344, 0x1);
__mv_accm344 = _mm_add_pd(__mv_accm344, __mv_hi);
y[357] = y[357]+__mv_accm344[0];;
  // stmt vectorized: y[357] += A[885] * x[358];
  __mv_accm345 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1047 = _mm256_maskload_pd(&A[((i0 + 0) + 886)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1048 = _mm256_maskload_pd(&x[((i0 + 0) + 357)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm345 = _mm256_fmadd_pd(__mv_vop1047, __mv_vop1048, __mv_accm345);	// latency = 5
    // stmt vectorized: y[0 * i0 + 358] += A[1 * i0 + 886] * x[1 * i0 + 357];
  }__mv_accm339 = _mm256_hadd_pd(__mv_accm339, __mv_accm341);	// latency = 6
__mv_accm339 = _mm256_add_pd(_mm256_blend_pd(__mv_accm339,__mv_accm343,0b1100), _mm256_permute2f128_pd(__mv_accm339,__mv_accm343,0x21));	// latency = 4
y[352] = y[352] + __mv_accm339[0];	// latency = 1
y[354] = y[354] + __mv_accm339[1];	// latency = 1
y[356] = y[356] + __mv_accm339[2];	// latency = 1
y[358] = y[358] + __mv_accm339[3];	// latency = 1

  __mv_vop1050 = _mm_loadu_pd(&A[889]);	// latency = 1
  __mv_vop1051 = _mm_i64gather_pd(&x[358], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm346 = _mm_fmadd_pd(__mv_vop1050, __mv_vop1051, __mv_accm346);	// latency = 5
  // stmt vectorized: y[359] += A[889] * x[358]
__mv_hi = _mm_shuffle_pd(__mv_accm346, __mv_accm346, 0x1);
__mv_accm346 = _mm_add_pd(__mv_accm346, __mv_hi);
y[359] = y[359]+__mv_accm346[0];;
  // stmt vectorized: y[359] += A[890] * x[360];
  __mv_accm347 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1053 = _mm256_maskload_pd(&A[((i0 + 0) + 891)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1054 = _mm256_maskload_pd(&x[((i0 + 0) + 359)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm347 = _mm256_fmadd_pd(__mv_vop1053, __mv_vop1054, __mv_accm347);	// latency = 5
    // stmt vectorized: y[0 * i0 + 360] += A[1 * i0 + 891] * x[1 * i0 + 359];
  }
  __mv_vop1056 = _mm_loadu_pd(&A[894]);	// latency = 1
  __mv_vop1057 = _mm_i64gather_pd(&x[360], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm348 = _mm_fmadd_pd(__mv_vop1056, __mv_vop1057, __mv_accm348);	// latency = 5
  // stmt vectorized: y[361] += A[894] * x[360]
__mv_hi = _mm_shuffle_pd(__mv_accm348, __mv_accm348, 0x1);
__mv_accm348 = _mm_add_pd(__mv_accm348, __mv_hi);
y[361] = y[361]+__mv_accm348[0];;
  // stmt vectorized: y[361] += A[895] * x[362];
  __mv_accm349 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1059 = _mm256_maskload_pd(&A[((i0 + 0) + 896)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1060 = _mm256_maskload_pd(&x[((i0 + 0) + 361)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm349 = _mm256_fmadd_pd(__mv_vop1059, __mv_vop1060, __mv_accm349);	// latency = 5
    // stmt vectorized: y[0 * i0 + 362] += A[1 * i0 + 896] * x[1 * i0 + 361];
  }
  __mv_vop1062 = _mm_loadu_pd(&A[899]);	// latency = 1
  __mv_vop1063 = _mm_i64gather_pd(&x[362], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm350 = _mm_fmadd_pd(__mv_vop1062, __mv_vop1063, __mv_accm350);	// latency = 5
  // stmt vectorized: y[363] += A[899] * x[362]
__mv_hi = _mm_shuffle_pd(__mv_accm350, __mv_accm350, 0x1);
__mv_accm350 = _mm_add_pd(__mv_accm350, __mv_hi);
y[363] = y[363]+__mv_accm350[0];;
  // stmt vectorized: y[363] += A[900] * x[364];
  __mv_accm351 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1065 = _mm256_maskload_pd(&A[((i0 + 0) + 901)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1066 = _mm256_maskload_pd(&x[((i0 + 0) + 363)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm351 = _mm256_fmadd_pd(__mv_vop1065, __mv_vop1066, __mv_accm351);	// latency = 5
    // stmt vectorized: y[0 * i0 + 364] += A[1 * i0 + 901] * x[1 * i0 + 363];
  }
  __mv_vop1068 = _mm_loadu_pd(&A[904]);	// latency = 1
  __mv_vop1069 = _mm_i64gather_pd(&x[364], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm352 = _mm_fmadd_pd(__mv_vop1068, __mv_vop1069, __mv_accm352);	// latency = 5
  // stmt vectorized: y[365] += A[904] * x[364]
__mv_hi = _mm_shuffle_pd(__mv_accm352, __mv_accm352, 0x1);
__mv_accm352 = _mm_add_pd(__mv_accm352, __mv_hi);
y[365] = y[365]+__mv_accm352[0];;
  // stmt vectorized: y[365] += A[905] * x[366];
  __mv_accm353 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1071 = _mm256_maskload_pd(&A[((i0 + 0) + 906)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1072 = _mm256_maskload_pd(&x[((i0 + 0) + 365)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm353 = _mm256_fmadd_pd(__mv_vop1071, __mv_vop1072, __mv_accm353);	// latency = 5
    // stmt vectorized: y[0 * i0 + 366] += A[1 * i0 + 906] * x[1 * i0 + 365];
  }__mv_accm347 = _mm256_hadd_pd(__mv_accm347, __mv_accm349);	// latency = 6
__mv_accm347 = _mm256_add_pd(_mm256_blend_pd(__mv_accm347,__mv_accm351,0b1100), _mm256_permute2f128_pd(__mv_accm347,__mv_accm351,0x21));	// latency = 4
y[360] = y[360] + __mv_accm347[0];	// latency = 1
y[362] = y[362] + __mv_accm347[1];	// latency = 1
y[364] = y[364] + __mv_accm347[2];	// latency = 1
y[366] = y[366] + __mv_accm347[3];	// latency = 1

  __mv_vop1074 = _mm_loadu_pd(&A[909]);	// latency = 1
  __mv_vop1075 = _mm_i64gather_pd(&x[366], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm354 = _mm_fmadd_pd(__mv_vop1074, __mv_vop1075, __mv_accm354);	// latency = 5
  // stmt vectorized: y[367] += A[909] * x[366]
__mv_hi = _mm_shuffle_pd(__mv_accm354, __mv_accm354, 0x1);
__mv_accm354 = _mm_add_pd(__mv_accm354, __mv_hi);
y[367] = y[367]+__mv_accm354[0];;
  // stmt vectorized: y[367] += A[910] * x[368];
  __mv_accm355 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1077 = _mm256_maskload_pd(&A[((i0 + 0) + 911)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1078 = _mm256_maskload_pd(&x[((i0 + 0) + 367)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm355 = _mm256_fmadd_pd(__mv_vop1077, __mv_vop1078, __mv_accm355);	// latency = 5
    // stmt vectorized: y[0 * i0 + 368] += A[1 * i0 + 911] * x[1 * i0 + 367];
  }
  __mv_vop1080 = _mm_loadu_pd(&A[914]);	// latency = 1
  __mv_vop1081 = _mm_i64gather_pd(&x[368], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm356 = _mm_fmadd_pd(__mv_vop1080, __mv_vop1081, __mv_accm356);	// latency = 5
  // stmt vectorized: y[369] += A[914] * x[368]
__mv_hi = _mm_shuffle_pd(__mv_accm356, __mv_accm356, 0x1);
__mv_accm356 = _mm_add_pd(__mv_accm356, __mv_hi);
y[369] = y[369]+__mv_accm356[0];;
  // stmt vectorized: y[369] += A[915] * x[370];
  __mv_accm357 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1083 = _mm256_maskload_pd(&A[((i0 + 0) + 916)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1084 = _mm256_maskload_pd(&x[((i0 + 0) + 369)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm357 = _mm256_fmadd_pd(__mv_vop1083, __mv_vop1084, __mv_accm357);	// latency = 5
    // stmt vectorized: y[0 * i0 + 370] += A[1 * i0 + 916] * x[1 * i0 + 369];
  }
  __mv_vop1086 = _mm_loadu_pd(&A[919]);	// latency = 1
  __mv_vop1087 = _mm_i64gather_pd(&x[370], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm358 = _mm_fmadd_pd(__mv_vop1086, __mv_vop1087, __mv_accm358);	// latency = 5
  // stmt vectorized: y[371] += A[919] * x[370]
__mv_hi = _mm_shuffle_pd(__mv_accm358, __mv_accm358, 0x1);
__mv_accm358 = _mm_add_pd(__mv_accm358, __mv_hi);
y[371] = y[371]+__mv_accm358[0];;
  // stmt vectorized: y[371] += A[920] * x[372];
  __mv_accm359 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1089 = _mm256_maskload_pd(&A[((i0 + 0) + 921)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1090 = _mm256_maskload_pd(&x[((i0 + 0) + 371)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm359 = _mm256_fmadd_pd(__mv_vop1089, __mv_vop1090, __mv_accm359);	// latency = 5
    // stmt vectorized: y[0 * i0 + 372] += A[1 * i0 + 921] * x[1 * i0 + 371];
  }
  __mv_vop1092 = _mm_loadu_pd(&A[924]);	// latency = 1
  __mv_vop1093 = _mm_i64gather_pd(&x[372], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm360 = _mm_fmadd_pd(__mv_vop1092, __mv_vop1093, __mv_accm360);	// latency = 5
  // stmt vectorized: y[373] += A[924] * x[372]
__mv_hi = _mm_shuffle_pd(__mv_accm360, __mv_accm360, 0x1);
__mv_accm360 = _mm_add_pd(__mv_accm360, __mv_hi);
y[373] = y[373]+__mv_accm360[0];;
  // stmt vectorized: y[373] += A[925] * x[374];
  __mv_accm361 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1095 = _mm256_maskload_pd(&A[((i0 + 0) + 926)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1096 = _mm256_maskload_pd(&x[((i0 + 0) + 373)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm361 = _mm256_fmadd_pd(__mv_vop1095, __mv_vop1096, __mv_accm361);	// latency = 5
    // stmt vectorized: y[0 * i0 + 374] += A[1 * i0 + 926] * x[1 * i0 + 373];
  }__mv_accm355 = _mm256_hadd_pd(__mv_accm355, __mv_accm357);	// latency = 6
__mv_accm355 = _mm256_add_pd(_mm256_blend_pd(__mv_accm355,__mv_accm359,0b1100), _mm256_permute2f128_pd(__mv_accm355,__mv_accm359,0x21));	// latency = 4
y[368] = y[368] + __mv_accm355[0];	// latency = 1
y[370] = y[370] + __mv_accm355[1];	// latency = 1
y[372] = y[372] + __mv_accm355[2];	// latency = 1
y[374] = y[374] + __mv_accm355[3];	// latency = 1

  __mv_vop1098 = _mm_loadu_pd(&A[929]);	// latency = 1
  __mv_vop1099 = _mm_i64gather_pd(&x[374], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm362 = _mm_fmadd_pd(__mv_vop1098, __mv_vop1099, __mv_accm362);	// latency = 5
  // stmt vectorized: y[375] += A[929] * x[374]
__mv_hi = _mm_shuffle_pd(__mv_accm362, __mv_accm362, 0x1);
__mv_accm362 = _mm_add_pd(__mv_accm362, __mv_hi);
y[375] = y[375]+__mv_accm362[0];;
  // stmt vectorized: y[375] += A[930] * x[376];
  __mv_accm363 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1101 = _mm256_maskload_pd(&A[((i0 + 0) + 931)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1102 = _mm256_maskload_pd(&x[((i0 + 0) + 375)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm363 = _mm256_fmadd_pd(__mv_vop1101, __mv_vop1102, __mv_accm363);	// latency = 5
    // stmt vectorized: y[0 * i0 + 376] += A[1 * i0 + 931] * x[1 * i0 + 375];
  }
  __mv_vop1104 = _mm_loadu_pd(&A[934]);	// latency = 1
  __mv_vop1105 = _mm_i64gather_pd(&x[376], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm364 = _mm_fmadd_pd(__mv_vop1104, __mv_vop1105, __mv_accm364);	// latency = 5
  // stmt vectorized: y[377] += A[934] * x[376]
__mv_hi = _mm_shuffle_pd(__mv_accm364, __mv_accm364, 0x1);
__mv_accm364 = _mm_add_pd(__mv_accm364, __mv_hi);
y[377] = y[377]+__mv_accm364[0];;
  // stmt vectorized: y[377] += A[935] * x[378];
  __mv_accm365 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1107 = _mm256_maskload_pd(&A[((i0 + 0) + 936)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1108 = _mm256_maskload_pd(&x[((i0 + 0) + 377)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm365 = _mm256_fmadd_pd(__mv_vop1107, __mv_vop1108, __mv_accm365);	// latency = 5
    // stmt vectorized: y[0 * i0 + 378] += A[1 * i0 + 936] * x[1 * i0 + 377];
  }
  __mv_vop1110 = _mm_loadu_pd(&A[939]);	// latency = 1
  __mv_vop1111 = _mm_i64gather_pd(&x[378], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm366 = _mm_fmadd_pd(__mv_vop1110, __mv_vop1111, __mv_accm366);	// latency = 5
  // stmt vectorized: y[379] += A[939] * x[378]
__mv_hi = _mm_shuffle_pd(__mv_accm366, __mv_accm366, 0x1);
__mv_accm366 = _mm_add_pd(__mv_accm366, __mv_hi);
y[379] = y[379]+__mv_accm366[0];;
  // stmt vectorized: y[379] += A[940] * x[380];
  __mv_accm367 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1113 = _mm256_maskload_pd(&A[((i0 + 0) + 941)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1114 = _mm256_maskload_pd(&x[((i0 + 0) + 379)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm367 = _mm256_fmadd_pd(__mv_vop1113, __mv_vop1114, __mv_accm367);	// latency = 5
    // stmt vectorized: y[0 * i0 + 380] += A[1 * i0 + 941] * x[1 * i0 + 379];
  }
  __mv_vop1116 = _mm_loadu_pd(&A[944]);	// latency = 1
  __mv_vop1117 = _mm_i64gather_pd(&x[380], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm368 = _mm_fmadd_pd(__mv_vop1116, __mv_vop1117, __mv_accm368);	// latency = 5
  // stmt vectorized: y[381] += A[944] * x[380]
__mv_hi = _mm_shuffle_pd(__mv_accm368, __mv_accm368, 0x1);
__mv_accm368 = _mm_add_pd(__mv_accm368, __mv_hi);
y[381] = y[381]+__mv_accm368[0];;
  // stmt vectorized: y[381] += A[945] * x[382];
  __mv_accm369 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1119 = _mm256_maskload_pd(&A[((i0 + 0) + 946)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1120 = _mm256_maskload_pd(&x[((i0 + 0) + 381)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm369 = _mm256_fmadd_pd(__mv_vop1119, __mv_vop1120, __mv_accm369);	// latency = 5
    // stmt vectorized: y[0 * i0 + 382] += A[1 * i0 + 946] * x[1 * i0 + 381];
  }__mv_accm363 = _mm256_hadd_pd(__mv_accm363, __mv_accm365);	// latency = 6
__mv_accm363 = _mm256_add_pd(_mm256_blend_pd(__mv_accm363,__mv_accm367,0b1100), _mm256_permute2f128_pd(__mv_accm363,__mv_accm367,0x21));	// latency = 4
y[376] = y[376] + __mv_accm363[0];	// latency = 1
y[378] = y[378] + __mv_accm363[1];	// latency = 1
y[380] = y[380] + __mv_accm363[2];	// latency = 1
y[382] = y[382] + __mv_accm363[3];	// latency = 1

  __mv_vop1122 = _mm_loadu_pd(&A[949]);	// latency = 1
  __mv_vop1123 = _mm_i64gather_pd(&x[382], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm370 = _mm_fmadd_pd(__mv_vop1122, __mv_vop1123, __mv_accm370);	// latency = 5
  // stmt vectorized: y[383] += A[949] * x[382]
__mv_hi = _mm_shuffle_pd(__mv_accm370, __mv_accm370, 0x1);
__mv_accm370 = _mm_add_pd(__mv_accm370, __mv_hi);
y[383] = y[383]+__mv_accm370[0];;
  // stmt vectorized: y[383] += A[950] * x[384];
  __mv_accm371 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1125 = _mm256_maskload_pd(&A[((i0 + 0) + 951)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1126 = _mm256_maskload_pd(&x[((i0 + 0) + 383)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm371 = _mm256_fmadd_pd(__mv_vop1125, __mv_vop1126, __mv_accm371);	// latency = 5
    // stmt vectorized: y[0 * i0 + 384] += A[1 * i0 + 951] * x[1 * i0 + 383];
  }
  __mv_vop1128 = _mm_loadu_pd(&A[954]);	// latency = 1
  __mv_vop1129 = _mm_i64gather_pd(&x[384], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm372 = _mm_fmadd_pd(__mv_vop1128, __mv_vop1129, __mv_accm372);	// latency = 5
  // stmt vectorized: y[385] += A[954] * x[384]
__mv_hi = _mm_shuffle_pd(__mv_accm372, __mv_accm372, 0x1);
__mv_accm372 = _mm_add_pd(__mv_accm372, __mv_hi);
y[385] = y[385]+__mv_accm372[0];;
  // stmt vectorized: y[385] += A[955] * x[386];
  __mv_accm373 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1131 = _mm256_maskload_pd(&A[((i0 + 0) + 956)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1132 = _mm256_maskload_pd(&x[((i0 + 0) + 385)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm373 = _mm256_fmadd_pd(__mv_vop1131, __mv_vop1132, __mv_accm373);	// latency = 5
    // stmt vectorized: y[0 * i0 + 386] += A[1 * i0 + 956] * x[1 * i0 + 385];
  }
  __mv_vop1134 = _mm_loadu_pd(&A[959]);	// latency = 1
  __mv_vop1135 = _mm_i64gather_pd(&x[386], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm374 = _mm_fmadd_pd(__mv_vop1134, __mv_vop1135, __mv_accm374);	// latency = 5
  // stmt vectorized: y[387] += A[959] * x[386]
__mv_hi = _mm_shuffle_pd(__mv_accm374, __mv_accm374, 0x1);
__mv_accm374 = _mm_add_pd(__mv_accm374, __mv_hi);
y[387] = y[387]+__mv_accm374[0];;
  // stmt vectorized: y[387] += A[960] * x[388];
  __mv_accm375 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1137 = _mm256_maskload_pd(&A[((i0 + 0) + 961)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1138 = _mm256_maskload_pd(&x[((i0 + 0) + 387)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm375 = _mm256_fmadd_pd(__mv_vop1137, __mv_vop1138, __mv_accm375);	// latency = 5
    // stmt vectorized: y[0 * i0 + 388] += A[1 * i0 + 961] * x[1 * i0 + 387];
  }
  __mv_vop1140 = _mm_loadu_pd(&A[964]);	// latency = 1
  __mv_vop1141 = _mm_i64gather_pd(&x[388], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm376 = _mm_fmadd_pd(__mv_vop1140, __mv_vop1141, __mv_accm376);	// latency = 5
  // stmt vectorized: y[389] += A[964] * x[388]
__mv_hi = _mm_shuffle_pd(__mv_accm376, __mv_accm376, 0x1);
__mv_accm376 = _mm_add_pd(__mv_accm376, __mv_hi);
y[389] = y[389]+__mv_accm376[0];;
  // stmt vectorized: y[389] += A[965] * x[390];
  __mv_accm377 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1143 = _mm256_maskload_pd(&A[((i0 + 0) + 966)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1144 = _mm256_maskload_pd(&x[((i0 + 0) + 389)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm377 = _mm256_fmadd_pd(__mv_vop1143, __mv_vop1144, __mv_accm377);	// latency = 5
    // stmt vectorized: y[0 * i0 + 390] += A[1 * i0 + 966] * x[1 * i0 + 389];
  }__mv_accm371 = _mm256_hadd_pd(__mv_accm371, __mv_accm373);	// latency = 6
__mv_accm371 = _mm256_add_pd(_mm256_blend_pd(__mv_accm371,__mv_accm375,0b1100), _mm256_permute2f128_pd(__mv_accm371,__mv_accm375,0x21));	// latency = 4
y[384] = y[384] + __mv_accm371[0];	// latency = 1
y[386] = y[386] + __mv_accm371[1];	// latency = 1
y[388] = y[388] + __mv_accm371[2];	// latency = 1
y[390] = y[390] + __mv_accm371[3];	// latency = 1

  __mv_vop1146 = _mm_loadu_pd(&A[969]);	// latency = 1
  __mv_vop1147 = _mm_i64gather_pd(&x[390], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm378 = _mm_fmadd_pd(__mv_vop1146, __mv_vop1147, __mv_accm378);	// latency = 5
  // stmt vectorized: y[391] += A[969] * x[390]
__mv_hi = _mm_shuffle_pd(__mv_accm378, __mv_accm378, 0x1);
__mv_accm378 = _mm_add_pd(__mv_accm378, __mv_hi);
y[391] = y[391]+__mv_accm378[0];;
  // stmt vectorized: y[391] += A[970] * x[392];
  __mv_accm379 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1149 = _mm256_maskload_pd(&A[((i0 + 0) + 971)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1150 = _mm256_maskload_pd(&x[((i0 + 0) + 391)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm379 = _mm256_fmadd_pd(__mv_vop1149, __mv_vop1150, __mv_accm379);	// latency = 5
    // stmt vectorized: y[0 * i0 + 392] += A[1 * i0 + 971] * x[1 * i0 + 391];
  }
  __mv_vop1152 = _mm_loadu_pd(&A[974]);	// latency = 1
  __mv_vop1153 = _mm_i64gather_pd(&x[392], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm380 = _mm_fmadd_pd(__mv_vop1152, __mv_vop1153, __mv_accm380);	// latency = 5
  // stmt vectorized: y[393] += A[974] * x[392]
__mv_hi = _mm_shuffle_pd(__mv_accm380, __mv_accm380, 0x1);
__mv_accm380 = _mm_add_pd(__mv_accm380, __mv_hi);
y[393] = y[393]+__mv_accm380[0];;
  // stmt vectorized: y[393] += A[975] * x[394];
  __mv_accm381 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1155 = _mm256_maskload_pd(&A[((i0 + 0) + 976)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1156 = _mm256_maskload_pd(&x[((i0 + 0) + 393)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm381 = _mm256_fmadd_pd(__mv_vop1155, __mv_vop1156, __mv_accm381);	// latency = 5
    // stmt vectorized: y[0 * i0 + 394] += A[1 * i0 + 976] * x[1 * i0 + 393];
  }
  __mv_vop1158 = _mm_loadu_pd(&A[979]);	// latency = 1
  __mv_vop1159 = _mm_i64gather_pd(&x[394], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm382 = _mm_fmadd_pd(__mv_vop1158, __mv_vop1159, __mv_accm382);	// latency = 5
  // stmt vectorized: y[395] += A[979] * x[394]
__mv_hi = _mm_shuffle_pd(__mv_accm382, __mv_accm382, 0x1);
__mv_accm382 = _mm_add_pd(__mv_accm382, __mv_hi);
y[395] = y[395]+__mv_accm382[0];;
  // stmt vectorized: y[395] += A[980] * x[396];
  __mv_accm383 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1161 = _mm256_maskload_pd(&A[((i0 + 0) + 981)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1162 = _mm256_maskload_pd(&x[((i0 + 0) + 395)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm383 = _mm256_fmadd_pd(__mv_vop1161, __mv_vop1162, __mv_accm383);	// latency = 5
    // stmt vectorized: y[0 * i0 + 396] += A[1 * i0 + 981] * x[1 * i0 + 395];
  }
  __mv_vop1164 = _mm_loadu_pd(&A[984]);	// latency = 1
  __mv_vop1165 = _mm_i64gather_pd(&x[396], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm384 = _mm_fmadd_pd(__mv_vop1164, __mv_vop1165, __mv_accm384);	// latency = 5
  // stmt vectorized: y[397] += A[984] * x[396]
__mv_hi = _mm_shuffle_pd(__mv_accm384, __mv_accm384, 0x1);
__mv_accm384 = _mm_add_pd(__mv_accm384, __mv_hi);
y[397] = y[397]+__mv_accm384[0];;
  // stmt vectorized: y[397] += A[985] * x[398];
  __mv_accm385 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1167 = _mm256_maskload_pd(&A[((i0 + 0) + 986)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1168 = _mm256_maskload_pd(&x[((i0 + 0) + 397)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm385 = _mm256_fmadd_pd(__mv_vop1167, __mv_vop1168, __mv_accm385);	// latency = 5
    // stmt vectorized: y[0 * i0 + 398] += A[1 * i0 + 986] * x[1 * i0 + 397];
  }__mv_accm379 = _mm256_hadd_pd(__mv_accm379, __mv_accm381);	// latency = 6
__mv_accm379 = _mm256_add_pd(_mm256_blend_pd(__mv_accm379,__mv_accm383,0b1100), _mm256_permute2f128_pd(__mv_accm379,__mv_accm383,0x21));	// latency = 4
y[392] = y[392] + __mv_accm379[0];	// latency = 1
y[394] = y[394] + __mv_accm379[1];	// latency = 1
y[396] = y[396] + __mv_accm379[2];	// latency = 1
y[398] = y[398] + __mv_accm379[3];	// latency = 1

  __mv_vop1170 = _mm_loadu_pd(&A[989]);	// latency = 1
  __mv_vop1171 = _mm_i64gather_pd(&x[398], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm386 = _mm_fmadd_pd(__mv_vop1170, __mv_vop1171, __mv_accm386);	// latency = 5
  // stmt vectorized: y[399] += A[989] * x[398]
__mv_hi = _mm_shuffle_pd(__mv_accm386, __mv_accm386, 0x1);
__mv_accm386 = _mm_add_pd(__mv_accm386, __mv_hi);
y[399] = y[399]+__mv_accm386[0];;
  // stmt vectorized: y[399] += A[990] * x[400];
  __mv_accm387 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1173 = _mm256_maskload_pd(&A[((i0 + 0) + 991)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1174 = _mm256_maskload_pd(&x[((i0 + 0) + 399)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm387 = _mm256_fmadd_pd(__mv_vop1173, __mv_vop1174, __mv_accm387);	// latency = 5
    // stmt vectorized: y[0 * i0 + 400] += A[1 * i0 + 991] * x[1 * i0 + 399];
  }
  __mv_vop1176 = _mm_loadu_pd(&A[994]);	// latency = 1
  __mv_vop1177 = _mm_i64gather_pd(&x[400], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm388 = _mm_fmadd_pd(__mv_vop1176, __mv_vop1177, __mv_accm388);	// latency = 5
  // stmt vectorized: y[401] += A[994] * x[400]
__mv_hi = _mm_shuffle_pd(__mv_accm388, __mv_accm388, 0x1);
__mv_accm388 = _mm_add_pd(__mv_accm388, __mv_hi);
y[401] = y[401]+__mv_accm388[0];;
  // stmt vectorized: y[401] += A[995] * x[402];
  __mv_accm389 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1179 = _mm256_maskload_pd(&A[((i0 + 0) + 996)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1180 = _mm256_maskload_pd(&x[((i0 + 0) + 401)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm389 = _mm256_fmadd_pd(__mv_vop1179, __mv_vop1180, __mv_accm389);	// latency = 5
    // stmt vectorized: y[0 * i0 + 402] += A[1 * i0 + 996] * x[1 * i0 + 401];
  }
  __mv_vop1182 = _mm_loadu_pd(&A[999]);	// latency = 1
  __mv_vop1183 = _mm_i64gather_pd(&x[402], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm390 = _mm_fmadd_pd(__mv_vop1182, __mv_vop1183, __mv_accm390);	// latency = 5
  // stmt vectorized: y[403] += A[999] * x[402]
__mv_hi = _mm_shuffle_pd(__mv_accm390, __mv_accm390, 0x1);
__mv_accm390 = _mm_add_pd(__mv_accm390, __mv_hi);
y[403] = y[403]+__mv_accm390[0];;
  // stmt vectorized: y[403] += A[1000] * x[404];
  __mv_accm391 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1185 = _mm256_maskload_pd(&A[((i0 + 0) + 1001)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1186 = _mm256_maskload_pd(&x[((i0 + 0) + 403)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm391 = _mm256_fmadd_pd(__mv_vop1185, __mv_vop1186, __mv_accm391);	// latency = 5
    // stmt vectorized: y[0 * i0 + 404] += A[1 * i0 + 1001] * x[1 * i0 + 403];
  }
  __mv_vop1188 = _mm_loadu_pd(&A[1004]);	// latency = 1
  __mv_vop1189 = _mm_i64gather_pd(&x[404], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm392 = _mm_fmadd_pd(__mv_vop1188, __mv_vop1189, __mv_accm392);	// latency = 5
  // stmt vectorized: y[405] += A[1004] * x[404]
__mv_hi = _mm_shuffle_pd(__mv_accm392, __mv_accm392, 0x1);
__mv_accm392 = _mm_add_pd(__mv_accm392, __mv_hi);
y[405] = y[405]+__mv_accm392[0];;
  // stmt vectorized: y[405] += A[1005] * x[406];
  __mv_accm393 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1191 = _mm256_maskload_pd(&A[((i0 + 0) + 1006)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1192 = _mm256_maskload_pd(&x[((i0 + 0) + 405)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm393 = _mm256_fmadd_pd(__mv_vop1191, __mv_vop1192, __mv_accm393);	// latency = 5
    // stmt vectorized: y[0 * i0 + 406] += A[1 * i0 + 1006] * x[1 * i0 + 405];
  }__mv_accm387 = _mm256_hadd_pd(__mv_accm387, __mv_accm389);	// latency = 6
__mv_accm387 = _mm256_add_pd(_mm256_blend_pd(__mv_accm387,__mv_accm391,0b1100), _mm256_permute2f128_pd(__mv_accm387,__mv_accm391,0x21));	// latency = 4
y[400] = y[400] + __mv_accm387[0];	// latency = 1
y[402] = y[402] + __mv_accm387[1];	// latency = 1
y[404] = y[404] + __mv_accm387[2];	// latency = 1
y[406] = y[406] + __mv_accm387[3];	// latency = 1

  __mv_vop1194 = _mm_loadu_pd(&A[1009]);	// latency = 1
  __mv_vop1195 = _mm_i64gather_pd(&x[406], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm394 = _mm_fmadd_pd(__mv_vop1194, __mv_vop1195, __mv_accm394);	// latency = 5
  // stmt vectorized: y[407] += A[1009] * x[406]
__mv_hi = _mm_shuffle_pd(__mv_accm394, __mv_accm394, 0x1);
__mv_accm394 = _mm_add_pd(__mv_accm394, __mv_hi);
y[407] = y[407]+__mv_accm394[0];;
  // stmt vectorized: y[407] += A[1010] * x[408];
  __mv_accm395 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1197 = _mm256_maskload_pd(&A[((i0 + 0) + 1011)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1198 = _mm256_maskload_pd(&x[((i0 + 0) + 407)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm395 = _mm256_fmadd_pd(__mv_vop1197, __mv_vop1198, __mv_accm395);	// latency = 5
    // stmt vectorized: y[0 * i0 + 408] += A[1 * i0 + 1011] * x[1 * i0 + 407];
  }
  __mv_vop1200 = _mm_loadu_pd(&A[1014]);	// latency = 1
  __mv_vop1201 = _mm_i64gather_pd(&x[408], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm396 = _mm_fmadd_pd(__mv_vop1200, __mv_vop1201, __mv_accm396);	// latency = 5
  // stmt vectorized: y[409] += A[1014] * x[408]
__mv_hi = _mm_shuffle_pd(__mv_accm396, __mv_accm396, 0x1);
__mv_accm396 = _mm_add_pd(__mv_accm396, __mv_hi);
y[409] = y[409]+__mv_accm396[0];;
  // stmt vectorized: y[409] += A[1015] * x[410];
  __mv_accm397 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1203 = _mm256_maskload_pd(&A[((i0 + 0) + 1016)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1204 = _mm256_maskload_pd(&x[((i0 + 0) + 409)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm397 = _mm256_fmadd_pd(__mv_vop1203, __mv_vop1204, __mv_accm397);	// latency = 5
    // stmt vectorized: y[0 * i0 + 410] += A[1 * i0 + 1016] * x[1 * i0 + 409];
  }
  __mv_vop1206 = _mm_loadu_pd(&A[1019]);	// latency = 1
  __mv_vop1207 = _mm_i64gather_pd(&x[410], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm398 = _mm_fmadd_pd(__mv_vop1206, __mv_vop1207, __mv_accm398);	// latency = 5
  // stmt vectorized: y[411] += A[1019] * x[410]
__mv_hi = _mm_shuffle_pd(__mv_accm398, __mv_accm398, 0x1);
__mv_accm398 = _mm_add_pd(__mv_accm398, __mv_hi);
y[411] = y[411]+__mv_accm398[0];;
  // stmt vectorized: y[411] += A[1020] * x[412];
  __mv_accm399 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1209 = _mm256_maskload_pd(&A[((i0 + 0) + 1021)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1210 = _mm256_maskload_pd(&x[((i0 + 0) + 411)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm399 = _mm256_fmadd_pd(__mv_vop1209, __mv_vop1210, __mv_accm399);	// latency = 5
    // stmt vectorized: y[0 * i0 + 412] += A[1 * i0 + 1021] * x[1 * i0 + 411];
  }
  __mv_vop1212 = _mm_loadu_pd(&A[1024]);	// latency = 1
  __mv_vop1213 = _mm_i64gather_pd(&x[412], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm400 = _mm_fmadd_pd(__mv_vop1212, __mv_vop1213, __mv_accm400);	// latency = 5
  // stmt vectorized: y[413] += A[1024] * x[412]
__mv_hi = _mm_shuffle_pd(__mv_accm400, __mv_accm400, 0x1);
__mv_accm400 = _mm_add_pd(__mv_accm400, __mv_hi);
y[413] = y[413]+__mv_accm400[0];;
  // stmt vectorized: y[413] += A[1025] * x[414];
  __mv_accm401 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1215 = _mm256_maskload_pd(&A[((i0 + 0) + 1026)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1216 = _mm256_maskload_pd(&x[((i0 + 0) + 413)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm401 = _mm256_fmadd_pd(__mv_vop1215, __mv_vop1216, __mv_accm401);	// latency = 5
    // stmt vectorized: y[0 * i0 + 414] += A[1 * i0 + 1026] * x[1 * i0 + 413];
  }__mv_accm395 = _mm256_hadd_pd(__mv_accm395, __mv_accm397);	// latency = 6
__mv_accm395 = _mm256_add_pd(_mm256_blend_pd(__mv_accm395,__mv_accm399,0b1100), _mm256_permute2f128_pd(__mv_accm395,__mv_accm399,0x21));	// latency = 4
y[408] = y[408] + __mv_accm395[0];	// latency = 1
y[410] = y[410] + __mv_accm395[1];	// latency = 1
y[412] = y[412] + __mv_accm395[2];	// latency = 1
y[414] = y[414] + __mv_accm395[3];	// latency = 1

  __mv_vop1218 = _mm_loadu_pd(&A[1029]);	// latency = 1
  __mv_vop1219 = _mm_i64gather_pd(&x[414], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm402 = _mm_fmadd_pd(__mv_vop1218, __mv_vop1219, __mv_accm402);	// latency = 5
  // stmt vectorized: y[415] += A[1029] * x[414]
__mv_hi = _mm_shuffle_pd(__mv_accm402, __mv_accm402, 0x1);
__mv_accm402 = _mm_add_pd(__mv_accm402, __mv_hi);
y[415] = y[415]+__mv_accm402[0];;
  // stmt vectorized: y[415] += A[1030] * x[416];
  __mv_accm403 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1221 = _mm256_maskload_pd(&A[((i0 + 0) + 1031)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1222 = _mm256_maskload_pd(&x[((i0 + 0) + 415)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm403 = _mm256_fmadd_pd(__mv_vop1221, __mv_vop1222, __mv_accm403);	// latency = 5
    // stmt vectorized: y[0 * i0 + 416] += A[1 * i0 + 1031] * x[1 * i0 + 415];
  }
  __mv_vop1224 = _mm_loadu_pd(&A[1034]);	// latency = 1
  __mv_vop1225 = _mm_i64gather_pd(&x[416], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm404 = _mm_fmadd_pd(__mv_vop1224, __mv_vop1225, __mv_accm404);	// latency = 5
  // stmt vectorized: y[417] += A[1034] * x[416]
__mv_hi = _mm_shuffle_pd(__mv_accm404, __mv_accm404, 0x1);
__mv_accm404 = _mm_add_pd(__mv_accm404, __mv_hi);
y[417] = y[417]+__mv_accm404[0];;
  // stmt vectorized: y[417] += A[1035] * x[418];
  __mv_accm405 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1227 = _mm256_maskload_pd(&A[((i0 + 0) + 1036)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1228 = _mm256_maskload_pd(&x[((i0 + 0) + 417)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm405 = _mm256_fmadd_pd(__mv_vop1227, __mv_vop1228, __mv_accm405);	// latency = 5
    // stmt vectorized: y[0 * i0 + 418] += A[1 * i0 + 1036] * x[1 * i0 + 417];
  }
  __mv_vop1230 = _mm_loadu_pd(&A[1039]);	// latency = 1
  __mv_vop1231 = _mm_i64gather_pd(&x[418], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm406 = _mm_fmadd_pd(__mv_vop1230, __mv_vop1231, __mv_accm406);	// latency = 5
  // stmt vectorized: y[419] += A[1039] * x[418]
__mv_hi = _mm_shuffle_pd(__mv_accm406, __mv_accm406, 0x1);
__mv_accm406 = _mm_add_pd(__mv_accm406, __mv_hi);
y[419] = y[419]+__mv_accm406[0];;
  // stmt vectorized: y[419] += A[1040] * x[420];
  __mv_accm407 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1233 = _mm256_maskload_pd(&A[((i0 + 0) + 1041)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1234 = _mm256_maskload_pd(&x[((i0 + 0) + 419)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm407 = _mm256_fmadd_pd(__mv_vop1233, __mv_vop1234, __mv_accm407);	// latency = 5
    // stmt vectorized: y[0 * i0 + 420] += A[1 * i0 + 1041] * x[1 * i0 + 419];
  }
  __mv_vop1236 = _mm_loadu_pd(&A[1044]);	// latency = 1
  __mv_vop1237 = _mm_i64gather_pd(&x[420], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm408 = _mm_fmadd_pd(__mv_vop1236, __mv_vop1237, __mv_accm408);	// latency = 5
  // stmt vectorized: y[421] += A[1044] * x[420]
__mv_hi = _mm_shuffle_pd(__mv_accm408, __mv_accm408, 0x1);
__mv_accm408 = _mm_add_pd(__mv_accm408, __mv_hi);
y[421] = y[421]+__mv_accm408[0];;
  // stmt vectorized: y[421] += A[1045] * x[422];
  __mv_accm409 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1239 = _mm256_maskload_pd(&A[((i0 + 0) + 1046)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1240 = _mm256_maskload_pd(&x[((i0 + 0) + 421)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm409 = _mm256_fmadd_pd(__mv_vop1239, __mv_vop1240, __mv_accm409);	// latency = 5
    // stmt vectorized: y[0 * i0 + 422] += A[1 * i0 + 1046] * x[1 * i0 + 421];
  }__mv_accm403 = _mm256_hadd_pd(__mv_accm403, __mv_accm405);	// latency = 6
__mv_accm403 = _mm256_add_pd(_mm256_blend_pd(__mv_accm403,__mv_accm407,0b1100), _mm256_permute2f128_pd(__mv_accm403,__mv_accm407,0x21));	// latency = 4
y[416] = y[416] + __mv_accm403[0];	// latency = 1
y[418] = y[418] + __mv_accm403[1];	// latency = 1
y[420] = y[420] + __mv_accm403[2];	// latency = 1
y[422] = y[422] + __mv_accm403[3];	// latency = 1

  __mv_vop1242 = _mm_loadu_pd(&A[1049]);	// latency = 1
  __mv_vop1243 = _mm_i64gather_pd(&x[422], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm410 = _mm_fmadd_pd(__mv_vop1242, __mv_vop1243, __mv_accm410);	// latency = 5
  // stmt vectorized: y[423] += A[1049] * x[422]
__mv_hi = _mm_shuffle_pd(__mv_accm410, __mv_accm410, 0x1);
__mv_accm410 = _mm_add_pd(__mv_accm410, __mv_hi);
y[423] = y[423]+__mv_accm410[0];;
  // stmt vectorized: y[423] += A[1050] * x[424];
  __mv_accm411 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1245 = _mm256_maskload_pd(&A[((i0 + 0) + 1051)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1246 = _mm256_maskload_pd(&x[((i0 + 0) + 423)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm411 = _mm256_fmadd_pd(__mv_vop1245, __mv_vop1246, __mv_accm411);	// latency = 5
    // stmt vectorized: y[0 * i0 + 424] += A[1 * i0 + 1051] * x[1 * i0 + 423];
  }
  __mv_vop1248 = _mm_loadu_pd(&A[1054]);	// latency = 1
  __mv_vop1249 = _mm_i64gather_pd(&x[424], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm412 = _mm_fmadd_pd(__mv_vop1248, __mv_vop1249, __mv_accm412);	// latency = 5
  // stmt vectorized: y[425] += A[1054] * x[424]
__mv_hi = _mm_shuffle_pd(__mv_accm412, __mv_accm412, 0x1);
__mv_accm412 = _mm_add_pd(__mv_accm412, __mv_hi);
y[425] = y[425]+__mv_accm412[0];;
  // stmt vectorized: y[425] += A[1055] * x[426];
  __mv_accm413 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1251 = _mm256_maskload_pd(&A[((i0 + 0) + 1056)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1252 = _mm256_maskload_pd(&x[((i0 + 0) + 425)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm413 = _mm256_fmadd_pd(__mv_vop1251, __mv_vop1252, __mv_accm413);	// latency = 5
    // stmt vectorized: y[0 * i0 + 426] += A[1 * i0 + 1056] * x[1 * i0 + 425];
  }
  __mv_vop1254 = _mm_loadu_pd(&A[1059]);	// latency = 1
  __mv_vop1255 = _mm_i64gather_pd(&x[426], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm414 = _mm_fmadd_pd(__mv_vop1254, __mv_vop1255, __mv_accm414);	// latency = 5
  // stmt vectorized: y[427] += A[1059] * x[426]
__mv_hi = _mm_shuffle_pd(__mv_accm414, __mv_accm414, 0x1);
__mv_accm414 = _mm_add_pd(__mv_accm414, __mv_hi);
y[427] = y[427]+__mv_accm414[0];;
  // stmt vectorized: y[427] += A[1060] * x[428];
  __mv_accm415 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1257 = _mm256_maskload_pd(&A[((i0 + 0) + 1061)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1258 = _mm256_maskload_pd(&x[((i0 + 0) + 427)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm415 = _mm256_fmadd_pd(__mv_vop1257, __mv_vop1258, __mv_accm415);	// latency = 5
    // stmt vectorized: y[0 * i0 + 428] += A[1 * i0 + 1061] * x[1 * i0 + 427];
  }
  __mv_vop1260 = _mm_loadu_pd(&A[1064]);	// latency = 1
  __mv_vop1261 = _mm_i64gather_pd(&x[428], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm416 = _mm_fmadd_pd(__mv_vop1260, __mv_vop1261, __mv_accm416);	// latency = 5
  // stmt vectorized: y[429] += A[1064] * x[428]
__mv_hi = _mm_shuffle_pd(__mv_accm416, __mv_accm416, 0x1);
__mv_accm416 = _mm_add_pd(__mv_accm416, __mv_hi);
y[429] = y[429]+__mv_accm416[0];;
  // stmt vectorized: y[429] += A[1065] * x[430];
  __mv_accm417 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1263 = _mm256_maskload_pd(&A[((i0 + 0) + 1066)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1264 = _mm256_maskload_pd(&x[((i0 + 0) + 429)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm417 = _mm256_fmadd_pd(__mv_vop1263, __mv_vop1264, __mv_accm417);	// latency = 5
    // stmt vectorized: y[0 * i0 + 430] += A[1 * i0 + 1066] * x[1 * i0 + 429];
  }__mv_accm411 = _mm256_hadd_pd(__mv_accm411, __mv_accm413);	// latency = 6
__mv_accm411 = _mm256_add_pd(_mm256_blend_pd(__mv_accm411,__mv_accm415,0b1100), _mm256_permute2f128_pd(__mv_accm411,__mv_accm415,0x21));	// latency = 4
y[424] = y[424] + __mv_accm411[0];	// latency = 1
y[426] = y[426] + __mv_accm411[1];	// latency = 1
y[428] = y[428] + __mv_accm411[2];	// latency = 1
y[430] = y[430] + __mv_accm411[3];	// latency = 1

  __mv_vop1266 = _mm_loadu_pd(&A[1069]);	// latency = 1
  __mv_vop1267 = _mm_i64gather_pd(&x[430], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm418 = _mm_fmadd_pd(__mv_vop1266, __mv_vop1267, __mv_accm418);	// latency = 5
  // stmt vectorized: y[431] += A[1069] * x[430]
__mv_hi = _mm_shuffle_pd(__mv_accm418, __mv_accm418, 0x1);
__mv_accm418 = _mm_add_pd(__mv_accm418, __mv_hi);
y[431] = y[431]+__mv_accm418[0];;
  // stmt vectorized: y[431] += A[1070] * x[432];
  __mv_accm419 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1269 = _mm256_maskload_pd(&A[((i0 + 0) + 1071)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1270 = _mm256_maskload_pd(&x[((i0 + 0) + 431)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm419 = _mm256_fmadd_pd(__mv_vop1269, __mv_vop1270, __mv_accm419);	// latency = 5
    // stmt vectorized: y[0 * i0 + 432] += A[1 * i0 + 1071] * x[1 * i0 + 431];
  }
  __mv_vop1272 = _mm_loadu_pd(&A[1074]);	// latency = 1
  __mv_vop1273 = _mm_i64gather_pd(&x[432], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm420 = _mm_fmadd_pd(__mv_vop1272, __mv_vop1273, __mv_accm420);	// latency = 5
  // stmt vectorized: y[433] += A[1074] * x[432]
__mv_hi = _mm_shuffle_pd(__mv_accm420, __mv_accm420, 0x1);
__mv_accm420 = _mm_add_pd(__mv_accm420, __mv_hi);
y[433] = y[433]+__mv_accm420[0];;
  // stmt vectorized: y[433] += A[1075] * x[434];
  __mv_accm421 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1275 = _mm256_maskload_pd(&A[((i0 + 0) + 1076)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1276 = _mm256_maskload_pd(&x[((i0 + 0) + 433)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm421 = _mm256_fmadd_pd(__mv_vop1275, __mv_vop1276, __mv_accm421);	// latency = 5
    // stmt vectorized: y[0 * i0 + 434] += A[1 * i0 + 1076] * x[1 * i0 + 433];
  }
  __mv_vop1278 = _mm_loadu_pd(&A[1079]);	// latency = 1
  __mv_vop1279 = _mm_i64gather_pd(&x[434], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm422 = _mm_fmadd_pd(__mv_vop1278, __mv_vop1279, __mv_accm422);	// latency = 5
  // stmt vectorized: y[435] += A[1079] * x[434]
__mv_hi = _mm_shuffle_pd(__mv_accm422, __mv_accm422, 0x1);
__mv_accm422 = _mm_add_pd(__mv_accm422, __mv_hi);
y[435] = y[435]+__mv_accm422[0];;
  // stmt vectorized: y[435] += A[1080] * x[436];
  __mv_accm423 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1281 = _mm256_maskload_pd(&A[((i0 + 0) + 1081)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1282 = _mm256_maskload_pd(&x[((i0 + 0) + 435)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm423 = _mm256_fmadd_pd(__mv_vop1281, __mv_vop1282, __mv_accm423);	// latency = 5
    // stmt vectorized: y[0 * i0 + 436] += A[1 * i0 + 1081] * x[1 * i0 + 435];
  }
  __mv_vop1284 = _mm_loadu_pd(&A[1084]);	// latency = 1
  __mv_vop1285 = _mm_i64gather_pd(&x[436], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm424 = _mm_fmadd_pd(__mv_vop1284, __mv_vop1285, __mv_accm424);	// latency = 5
  // stmt vectorized: y[437] += A[1084] * x[436]
__mv_hi = _mm_shuffle_pd(__mv_accm424, __mv_accm424, 0x1);
__mv_accm424 = _mm_add_pd(__mv_accm424, __mv_hi);
y[437] = y[437]+__mv_accm424[0];;
  // stmt vectorized: y[437] += A[1085] * x[438];
  __mv_accm425 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1287 = _mm256_maskload_pd(&A[((i0 + 0) + 1086)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1288 = _mm256_maskload_pd(&x[((i0 + 0) + 437)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm425 = _mm256_fmadd_pd(__mv_vop1287, __mv_vop1288, __mv_accm425);	// latency = 5
    // stmt vectorized: y[0 * i0 + 438] += A[1 * i0 + 1086] * x[1 * i0 + 437];
  }__mv_accm419 = _mm256_hadd_pd(__mv_accm419, __mv_accm421);	// latency = 6
__mv_accm419 = _mm256_add_pd(_mm256_blend_pd(__mv_accm419,__mv_accm423,0b1100), _mm256_permute2f128_pd(__mv_accm419,__mv_accm423,0x21));	// latency = 4
y[432] = y[432] + __mv_accm419[0];	// latency = 1
y[434] = y[434] + __mv_accm419[1];	// latency = 1
y[436] = y[436] + __mv_accm419[2];	// latency = 1
y[438] = y[438] + __mv_accm419[3];	// latency = 1

  __mv_vop1290 = _mm_loadu_pd(&A[1089]);	// latency = 1
  __mv_vop1291 = _mm_i64gather_pd(&x[438], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm426 = _mm_fmadd_pd(__mv_vop1290, __mv_vop1291, __mv_accm426);	// latency = 5
  // stmt vectorized: y[439] += A[1089] * x[438]
__mv_hi = _mm_shuffle_pd(__mv_accm426, __mv_accm426, 0x1);
__mv_accm426 = _mm_add_pd(__mv_accm426, __mv_hi);
y[439] = y[439]+__mv_accm426[0];;
  // stmt vectorized: y[439] += A[1090] * x[440];
  __mv_accm427 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1293 = _mm256_maskload_pd(&A[((i0 + 0) + 1091)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1294 = _mm256_maskload_pd(&x[((i0 + 0) + 439)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm427 = _mm256_fmadd_pd(__mv_vop1293, __mv_vop1294, __mv_accm427);	// latency = 5
    // stmt vectorized: y[0 * i0 + 440] += A[1 * i0 + 1091] * x[1 * i0 + 439];
  }
  __mv_vop1296 = _mm_loadu_pd(&A[1094]);	// latency = 1
  __mv_vop1297 = _mm_i64gather_pd(&x[440], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm428 = _mm_fmadd_pd(__mv_vop1296, __mv_vop1297, __mv_accm428);	// latency = 5
  // stmt vectorized: y[441] += A[1094] * x[440]
__mv_hi = _mm_shuffle_pd(__mv_accm428, __mv_accm428, 0x1);
__mv_accm428 = _mm_add_pd(__mv_accm428, __mv_hi);
y[441] = y[441]+__mv_accm428[0];;
  // stmt vectorized: y[441] += A[1095] * x[442];
  __mv_accm429 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1299 = _mm256_maskload_pd(&A[((i0 + 0) + 1096)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1300 = _mm256_maskload_pd(&x[((i0 + 0) + 441)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm429 = _mm256_fmadd_pd(__mv_vop1299, __mv_vop1300, __mv_accm429);	// latency = 5
    // stmt vectorized: y[0 * i0 + 442] += A[1 * i0 + 1096] * x[1 * i0 + 441];
  }
  __mv_vop1302 = _mm_loadu_pd(&A[1099]);	// latency = 1
  __mv_vop1303 = _mm_i64gather_pd(&x[442], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm430 = _mm_fmadd_pd(__mv_vop1302, __mv_vop1303, __mv_accm430);	// latency = 5
  // stmt vectorized: y[443] += A[1099] * x[442]
__mv_hi = _mm_shuffle_pd(__mv_accm430, __mv_accm430, 0x1);
__mv_accm430 = _mm_add_pd(__mv_accm430, __mv_hi);
y[443] = y[443]+__mv_accm430[0];;
  // stmt vectorized: y[443] += A[1100] * x[444];
  __mv_accm431 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1305 = _mm256_maskload_pd(&A[((i0 + 0) + 1101)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1306 = _mm256_maskload_pd(&x[((i0 + 0) + 443)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm431 = _mm256_fmadd_pd(__mv_vop1305, __mv_vop1306, __mv_accm431);	// latency = 5
    // stmt vectorized: y[0 * i0 + 444] += A[1 * i0 + 1101] * x[1 * i0 + 443];
  }
  __mv_vop1308 = _mm_loadu_pd(&A[1104]);	// latency = 1
  __mv_vop1309 = _mm_i64gather_pd(&x[444], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm432 = _mm_fmadd_pd(__mv_vop1308, __mv_vop1309, __mv_accm432);	// latency = 5
  // stmt vectorized: y[445] += A[1104] * x[444]
__mv_hi = _mm_shuffle_pd(__mv_accm432, __mv_accm432, 0x1);
__mv_accm432 = _mm_add_pd(__mv_accm432, __mv_hi);
y[445] = y[445]+__mv_accm432[0];;
  // stmt vectorized: y[445] += A[1105] * x[446];
  __mv_accm433 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1311 = _mm256_maskload_pd(&A[((i0 + 0) + 1106)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1312 = _mm256_maskload_pd(&x[((i0 + 0) + 445)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm433 = _mm256_fmadd_pd(__mv_vop1311, __mv_vop1312, __mv_accm433);	// latency = 5
    // stmt vectorized: y[0 * i0 + 446] += A[1 * i0 + 1106] * x[1 * i0 + 445];
  }__mv_accm427 = _mm256_hadd_pd(__mv_accm427, __mv_accm429);	// latency = 6
__mv_accm427 = _mm256_add_pd(_mm256_blend_pd(__mv_accm427,__mv_accm431,0b1100), _mm256_permute2f128_pd(__mv_accm427,__mv_accm431,0x21));	// latency = 4
y[440] = y[440] + __mv_accm427[0];	// latency = 1
y[442] = y[442] + __mv_accm427[1];	// latency = 1
y[444] = y[444] + __mv_accm427[2];	// latency = 1
y[446] = y[446] + __mv_accm427[3];	// latency = 1

  __mv_vop1314 = _mm_loadu_pd(&A[1109]);	// latency = 1
  __mv_vop1315 = _mm_i64gather_pd(&x[446], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm434 = _mm_fmadd_pd(__mv_vop1314, __mv_vop1315, __mv_accm434);	// latency = 5
  // stmt vectorized: y[447] += A[1109] * x[446]
__mv_hi = _mm_shuffle_pd(__mv_accm434, __mv_accm434, 0x1);
__mv_accm434 = _mm_add_pd(__mv_accm434, __mv_hi);
y[447] = y[447]+__mv_accm434[0];;
  // stmt vectorized: y[447] += A[1110] * x[448];
  __mv_accm435 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1317 = _mm256_maskload_pd(&A[((i0 + 0) + 1111)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1318 = _mm256_maskload_pd(&x[((i0 + 0) + 447)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm435 = _mm256_fmadd_pd(__mv_vop1317, __mv_vop1318, __mv_accm435);	// latency = 5
    // stmt vectorized: y[0 * i0 + 448] += A[1 * i0 + 1111] * x[1 * i0 + 447];
  }
  __mv_vop1320 = _mm_loadu_pd(&A[1114]);	// latency = 1
  __mv_vop1321 = _mm_i64gather_pd(&x[448], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm436 = _mm_fmadd_pd(__mv_vop1320, __mv_vop1321, __mv_accm436);	// latency = 5
  // stmt vectorized: y[449] += A[1114] * x[448]
__mv_hi = _mm_shuffle_pd(__mv_accm436, __mv_accm436, 0x1);
__mv_accm436 = _mm_add_pd(__mv_accm436, __mv_hi);
y[449] = y[449]+__mv_accm436[0];;
  // stmt vectorized: y[449] += A[1115] * x[450];
  __mv_accm437 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1323 = _mm256_maskload_pd(&A[((i0 + 0) + 1116)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1324 = _mm256_maskload_pd(&x[((i0 + 0) + 449)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm437 = _mm256_fmadd_pd(__mv_vop1323, __mv_vop1324, __mv_accm437);	// latency = 5
    // stmt vectorized: y[0 * i0 + 450] += A[1 * i0 + 1116] * x[1 * i0 + 449];
  }
  __mv_vop1326 = _mm_loadu_pd(&A[1119]);	// latency = 1
  __mv_vop1327 = _mm_i64gather_pd(&x[450], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm438 = _mm_fmadd_pd(__mv_vop1326, __mv_vop1327, __mv_accm438);	// latency = 5
  // stmt vectorized: y[451] += A[1119] * x[450]
__mv_hi = _mm_shuffle_pd(__mv_accm438, __mv_accm438, 0x1);
__mv_accm438 = _mm_add_pd(__mv_accm438, __mv_hi);
y[451] = y[451]+__mv_accm438[0];;
  // stmt vectorized: y[451] += A[1120] * x[452];
  __mv_accm439 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1329 = _mm256_maskload_pd(&A[((i0 + 0) + 1121)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1330 = _mm256_maskload_pd(&x[((i0 + 0) + 451)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm439 = _mm256_fmadd_pd(__mv_vop1329, __mv_vop1330, __mv_accm439);	// latency = 5
    // stmt vectorized: y[0 * i0 + 452] += A[1 * i0 + 1121] * x[1 * i0 + 451];
  }
  __mv_vop1332 = _mm_loadu_pd(&A[1124]);	// latency = 1
  __mv_vop1333 = _mm_i64gather_pd(&x[452], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm440 = _mm_fmadd_pd(__mv_vop1332, __mv_vop1333, __mv_accm440);	// latency = 5
  // stmt vectorized: y[453] += A[1124] * x[452]
__mv_hi = _mm_shuffle_pd(__mv_accm440, __mv_accm440, 0x1);
__mv_accm440 = _mm_add_pd(__mv_accm440, __mv_hi);
y[453] = y[453]+__mv_accm440[0];;
  // stmt vectorized: y[453] += A[1125] * x[454];
  __mv_accm441 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1335 = _mm256_maskload_pd(&A[((i0 + 0) + 1126)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1336 = _mm256_maskload_pd(&x[((i0 + 0) + 453)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm441 = _mm256_fmadd_pd(__mv_vop1335, __mv_vop1336, __mv_accm441);	// latency = 5
    // stmt vectorized: y[0 * i0 + 454] += A[1 * i0 + 1126] * x[1 * i0 + 453];
  }__mv_accm435 = _mm256_hadd_pd(__mv_accm435, __mv_accm437);	// latency = 6
__mv_accm435 = _mm256_add_pd(_mm256_blend_pd(__mv_accm435,__mv_accm439,0b1100), _mm256_permute2f128_pd(__mv_accm435,__mv_accm439,0x21));	// latency = 4
y[448] = y[448] + __mv_accm435[0];	// latency = 1
y[450] = y[450] + __mv_accm435[1];	// latency = 1
y[452] = y[452] + __mv_accm435[2];	// latency = 1
y[454] = y[454] + __mv_accm435[3];	// latency = 1

  __mv_vop1338 = _mm_loadu_pd(&A[1129]);	// latency = 1
  __mv_vop1339 = _mm_i64gather_pd(&x[454], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm442 = _mm_fmadd_pd(__mv_vop1338, __mv_vop1339, __mv_accm442);	// latency = 5
  // stmt vectorized: y[455] += A[1129] * x[454]
__mv_hi = _mm_shuffle_pd(__mv_accm442, __mv_accm442, 0x1);
__mv_accm442 = _mm_add_pd(__mv_accm442, __mv_hi);
y[455] = y[455]+__mv_accm442[0];;
  // stmt vectorized: y[455] += A[1130] * x[456];
  __mv_accm443 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1341 = _mm256_maskload_pd(&A[((i0 + 0) + 1131)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1342 = _mm256_maskload_pd(&x[((i0 + 0) + 455)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm443 = _mm256_fmadd_pd(__mv_vop1341, __mv_vop1342, __mv_accm443);	// latency = 5
    // stmt vectorized: y[0 * i0 + 456] += A[1 * i0 + 1131] * x[1 * i0 + 455];
  }
  __mv_vop1344 = _mm_loadu_pd(&A[1134]);	// latency = 1
  __mv_vop1345 = _mm_i64gather_pd(&x[456], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm444 = _mm_fmadd_pd(__mv_vop1344, __mv_vop1345, __mv_accm444);	// latency = 5
  // stmt vectorized: y[457] += A[1134] * x[456]
__mv_hi = _mm_shuffle_pd(__mv_accm444, __mv_accm444, 0x1);
__mv_accm444 = _mm_add_pd(__mv_accm444, __mv_hi);
y[457] = y[457]+__mv_accm444[0];;
  // stmt vectorized: y[457] += A[1135] * x[458];
  __mv_accm445 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1347 = _mm256_maskload_pd(&A[((i0 + 0) + 1136)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1348 = _mm256_maskload_pd(&x[((i0 + 0) + 457)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm445 = _mm256_fmadd_pd(__mv_vop1347, __mv_vop1348, __mv_accm445);	// latency = 5
    // stmt vectorized: y[0 * i0 + 458] += A[1 * i0 + 1136] * x[1 * i0 + 457];
  }
  __mv_vop1350 = _mm_loadu_pd(&A[1139]);	// latency = 1
  __mv_vop1351 = _mm_i64gather_pd(&x[458], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm446 = _mm_fmadd_pd(__mv_vop1350, __mv_vop1351, __mv_accm446);	// latency = 5
  // stmt vectorized: y[459] += A[1139] * x[458]
__mv_hi = _mm_shuffle_pd(__mv_accm446, __mv_accm446, 0x1);
__mv_accm446 = _mm_add_pd(__mv_accm446, __mv_hi);
y[459] = y[459]+__mv_accm446[0];;
  // stmt vectorized: y[459] += A[1140] * x[460];
  __mv_accm447 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1353 = _mm256_maskload_pd(&A[((i0 + 0) + 1141)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1354 = _mm256_maskload_pd(&x[((i0 + 0) + 459)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm447 = _mm256_fmadd_pd(__mv_vop1353, __mv_vop1354, __mv_accm447);	// latency = 5
    // stmt vectorized: y[0 * i0 + 460] += A[1 * i0 + 1141] * x[1 * i0 + 459];
  }
  __mv_vop1356 = _mm_loadu_pd(&A[1144]);	// latency = 1
  __mv_vop1357 = _mm_i64gather_pd(&x[460], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm448 = _mm_fmadd_pd(__mv_vop1356, __mv_vop1357, __mv_accm448);	// latency = 5
  // stmt vectorized: y[461] += A[1144] * x[460]
__mv_hi = _mm_shuffle_pd(__mv_accm448, __mv_accm448, 0x1);
__mv_accm448 = _mm_add_pd(__mv_accm448, __mv_hi);
y[461] = y[461]+__mv_accm448[0];;
  // stmt vectorized: y[461] += A[1145] * x[462];
  __mv_accm449 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1359 = _mm256_maskload_pd(&A[((i0 + 0) + 1146)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1360 = _mm256_maskload_pd(&x[((i0 + 0) + 461)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm449 = _mm256_fmadd_pd(__mv_vop1359, __mv_vop1360, __mv_accm449);	// latency = 5
    // stmt vectorized: y[0 * i0 + 462] += A[1 * i0 + 1146] * x[1 * i0 + 461];
  }__mv_accm443 = _mm256_hadd_pd(__mv_accm443, __mv_accm445);	// latency = 6
__mv_accm443 = _mm256_add_pd(_mm256_blend_pd(__mv_accm443,__mv_accm447,0b1100), _mm256_permute2f128_pd(__mv_accm443,__mv_accm447,0x21));	// latency = 4
y[456] = y[456] + __mv_accm443[0];	// latency = 1
y[458] = y[458] + __mv_accm443[1];	// latency = 1
y[460] = y[460] + __mv_accm443[2];	// latency = 1
y[462] = y[462] + __mv_accm443[3];	// latency = 1

  __mv_vop1362 = _mm_loadu_pd(&A[1149]);	// latency = 1
  __mv_vop1363 = _mm_i64gather_pd(&x[462], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm450 = _mm_fmadd_pd(__mv_vop1362, __mv_vop1363, __mv_accm450);	// latency = 5
  // stmt vectorized: y[463] += A[1149] * x[462]
__mv_hi = _mm_shuffle_pd(__mv_accm450, __mv_accm450, 0x1);
__mv_accm450 = _mm_add_pd(__mv_accm450, __mv_hi);
y[463] = y[463]+__mv_accm450[0];;
  // stmt vectorized: y[463] += A[1150] * x[464];
  __mv_accm451 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1365 = _mm256_maskload_pd(&A[((i0 + 0) + 1151)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1366 = _mm256_maskload_pd(&x[((i0 + 0) + 463)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm451 = _mm256_fmadd_pd(__mv_vop1365, __mv_vop1366, __mv_accm451);	// latency = 5
    // stmt vectorized: y[0 * i0 + 464] += A[1 * i0 + 1151] * x[1 * i0 + 463];
  }
  __mv_vop1368 = _mm_loadu_pd(&A[1154]);	// latency = 1
  __mv_vop1369 = _mm_i64gather_pd(&x[464], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm452 = _mm_fmadd_pd(__mv_vop1368, __mv_vop1369, __mv_accm452);	// latency = 5
  // stmt vectorized: y[465] += A[1154] * x[464]
__mv_hi = _mm_shuffle_pd(__mv_accm452, __mv_accm452, 0x1);
__mv_accm452 = _mm_add_pd(__mv_accm452, __mv_hi);
y[465] = y[465]+__mv_accm452[0];;
  // stmt vectorized: y[465] += A[1155] * x[466];
  __mv_accm453 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1371 = _mm256_maskload_pd(&A[((i0 + 0) + 1156)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1372 = _mm256_maskload_pd(&x[((i0 + 0) + 465)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm453 = _mm256_fmadd_pd(__mv_vop1371, __mv_vop1372, __mv_accm453);	// latency = 5
    // stmt vectorized: y[0 * i0 + 466] += A[1 * i0 + 1156] * x[1 * i0 + 465];
  }
  __mv_vop1374 = _mm_loadu_pd(&A[1159]);	// latency = 1
  __mv_vop1375 = _mm_i64gather_pd(&x[466], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm454 = _mm_fmadd_pd(__mv_vop1374, __mv_vop1375, __mv_accm454);	// latency = 5
  // stmt vectorized: y[467] += A[1159] * x[466]
__mv_hi = _mm_shuffle_pd(__mv_accm454, __mv_accm454, 0x1);
__mv_accm454 = _mm_add_pd(__mv_accm454, __mv_hi);
y[467] = y[467]+__mv_accm454[0];;
  // stmt vectorized: y[467] += A[1160] * x[468];
  __mv_accm455 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1377 = _mm256_maskload_pd(&A[((i0 + 0) + 1161)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1378 = _mm256_maskload_pd(&x[((i0 + 0) + 467)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm455 = _mm256_fmadd_pd(__mv_vop1377, __mv_vop1378, __mv_accm455);	// latency = 5
    // stmt vectorized: y[0 * i0 + 468] += A[1 * i0 + 1161] * x[1 * i0 + 467];
  }
  __mv_vop1380 = _mm_loadu_pd(&A[1164]);	// latency = 1
  __mv_vop1381 = _mm_i64gather_pd(&x[468], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm456 = _mm_fmadd_pd(__mv_vop1380, __mv_vop1381, __mv_accm456);	// latency = 5
  // stmt vectorized: y[469] += A[1164] * x[468]
__mv_hi = _mm_shuffle_pd(__mv_accm456, __mv_accm456, 0x1);
__mv_accm456 = _mm_add_pd(__mv_accm456, __mv_hi);
y[469] = y[469]+__mv_accm456[0];;
  // stmt vectorized: y[469] += A[1165] * x[470];
  __mv_accm457 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1383 = _mm256_maskload_pd(&A[((i0 + 0) + 1166)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1384 = _mm256_maskload_pd(&x[((i0 + 0) + 469)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm457 = _mm256_fmadd_pd(__mv_vop1383, __mv_vop1384, __mv_accm457);	// latency = 5
    // stmt vectorized: y[0 * i0 + 470] += A[1 * i0 + 1166] * x[1 * i0 + 469];
  }__mv_accm451 = _mm256_hadd_pd(__mv_accm451, __mv_accm453);	// latency = 6
__mv_accm451 = _mm256_add_pd(_mm256_blend_pd(__mv_accm451,__mv_accm455,0b1100), _mm256_permute2f128_pd(__mv_accm451,__mv_accm455,0x21));	// latency = 4
y[464] = y[464] + __mv_accm451[0];	// latency = 1
y[466] = y[466] + __mv_accm451[1];	// latency = 1
y[468] = y[468] + __mv_accm451[2];	// latency = 1
y[470] = y[470] + __mv_accm451[3];	// latency = 1

  __mv_vop1386 = _mm_loadu_pd(&A[1169]);	// latency = 1
  __mv_vop1387 = _mm_i64gather_pd(&x[470], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm458 = _mm_fmadd_pd(__mv_vop1386, __mv_vop1387, __mv_accm458);	// latency = 5
  // stmt vectorized: y[471] += A[1169] * x[470]
__mv_hi = _mm_shuffle_pd(__mv_accm458, __mv_accm458, 0x1);
__mv_accm458 = _mm_add_pd(__mv_accm458, __mv_hi);
y[471] = y[471]+__mv_accm458[0];;
  // stmt vectorized: y[471] += A[1170] * x[472];
  __mv_accm459 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1389 = _mm256_maskload_pd(&A[((i0 + 0) + 1171)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1390 = _mm256_maskload_pd(&x[((i0 + 0) + 471)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm459 = _mm256_fmadd_pd(__mv_vop1389, __mv_vop1390, __mv_accm459);	// latency = 5
    // stmt vectorized: y[0 * i0 + 472] += A[1 * i0 + 1171] * x[1 * i0 + 471];
  }
  __mv_vop1392 = _mm_loadu_pd(&A[1174]);	// latency = 1
  __mv_vop1393 = _mm_i64gather_pd(&x[472], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm460 = _mm_fmadd_pd(__mv_vop1392, __mv_vop1393, __mv_accm460);	// latency = 5
  // stmt vectorized: y[473] += A[1174] * x[472]
__mv_hi = _mm_shuffle_pd(__mv_accm460, __mv_accm460, 0x1);
__mv_accm460 = _mm_add_pd(__mv_accm460, __mv_hi);
y[473] = y[473]+__mv_accm460[0];;
  // stmt vectorized: y[473] += A[1175] * x[474];
  __mv_accm461 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1395 = _mm256_maskload_pd(&A[((i0 + 0) + 1176)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1396 = _mm256_maskload_pd(&x[((i0 + 0) + 473)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm461 = _mm256_fmadd_pd(__mv_vop1395, __mv_vop1396, __mv_accm461);	// latency = 5
    // stmt vectorized: y[0 * i0 + 474] += A[1 * i0 + 1176] * x[1 * i0 + 473];
  }
  __mv_vop1398 = _mm_loadu_pd(&A[1179]);	// latency = 1
  __mv_vop1399 = _mm_i64gather_pd(&x[474], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm462 = _mm_fmadd_pd(__mv_vop1398, __mv_vop1399, __mv_accm462);	// latency = 5
  // stmt vectorized: y[475] += A[1179] * x[474]
__mv_hi = _mm_shuffle_pd(__mv_accm462, __mv_accm462, 0x1);
__mv_accm462 = _mm_add_pd(__mv_accm462, __mv_hi);
y[475] = y[475]+__mv_accm462[0];;
  // stmt vectorized: y[475] += A[1180] * x[476];
  __mv_accm463 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1401 = _mm256_maskload_pd(&A[((i0 + 0) + 1181)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1402 = _mm256_maskload_pd(&x[((i0 + 0) + 475)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm463 = _mm256_fmadd_pd(__mv_vop1401, __mv_vop1402, __mv_accm463);	// latency = 5
    // stmt vectorized: y[0 * i0 + 476] += A[1 * i0 + 1181] * x[1 * i0 + 475];
  }
  __mv_vop1404 = _mm_loadu_pd(&A[1184]);	// latency = 1
  __mv_vop1405 = _mm_i64gather_pd(&x[476], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm464 = _mm_fmadd_pd(__mv_vop1404, __mv_vop1405, __mv_accm464);	// latency = 5
  // stmt vectorized: y[477] += A[1184] * x[476]
__mv_hi = _mm_shuffle_pd(__mv_accm464, __mv_accm464, 0x1);
__mv_accm464 = _mm_add_pd(__mv_accm464, __mv_hi);
y[477] = y[477]+__mv_accm464[0];;
  // stmt vectorized: y[477] += A[1185] * x[478];
  __mv_accm465 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1407 = _mm256_maskload_pd(&A[((i0 + 0) + 1186)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1408 = _mm256_maskload_pd(&x[((i0 + 0) + 477)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm465 = _mm256_fmadd_pd(__mv_vop1407, __mv_vop1408, __mv_accm465);	// latency = 5
    // stmt vectorized: y[0 * i0 + 478] += A[1 * i0 + 1186] * x[1 * i0 + 477];
  }__mv_accm459 = _mm256_hadd_pd(__mv_accm459, __mv_accm461);	// latency = 6
__mv_accm459 = _mm256_add_pd(_mm256_blend_pd(__mv_accm459,__mv_accm463,0b1100), _mm256_permute2f128_pd(__mv_accm459,__mv_accm463,0x21));	// latency = 4
y[472] = y[472] + __mv_accm459[0];	// latency = 1
y[474] = y[474] + __mv_accm459[1];	// latency = 1
y[476] = y[476] + __mv_accm459[2];	// latency = 1
y[478] = y[478] + __mv_accm459[3];	// latency = 1

  __mv_vop1410 = _mm_loadu_pd(&A[1189]);	// latency = 1
  __mv_vop1411 = _mm_i64gather_pd(&x[478], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm466 = _mm_fmadd_pd(__mv_vop1410, __mv_vop1411, __mv_accm466);	// latency = 5
  // stmt vectorized: y[479] += A[1189] * x[478]
__mv_hi = _mm_shuffle_pd(__mv_accm466, __mv_accm466, 0x1);
__mv_accm466 = _mm_add_pd(__mv_accm466, __mv_hi);
y[479] = y[479]+__mv_accm466[0];;
  // stmt vectorized: y[479] += A[1190] * x[480];
  __mv_accm467 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1413 = _mm256_maskload_pd(&A[((i0 + 0) + 1191)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1414 = _mm256_maskload_pd(&x[((i0 + 0) + 479)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm467 = _mm256_fmadd_pd(__mv_vop1413, __mv_vop1414, __mv_accm467);	// latency = 5
    // stmt vectorized: y[0 * i0 + 480] += A[1 * i0 + 1191] * x[1 * i0 + 479];
  }
  __mv_vop1416 = _mm_loadu_pd(&A[1194]);	// latency = 1
  __mv_vop1417 = _mm_i64gather_pd(&x[480], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm468 = _mm_fmadd_pd(__mv_vop1416, __mv_vop1417, __mv_accm468);	// latency = 5
  // stmt vectorized: y[481] += A[1194] * x[480]
__mv_hi = _mm_shuffle_pd(__mv_accm468, __mv_accm468, 0x1);
__mv_accm468 = _mm_add_pd(__mv_accm468, __mv_hi);
y[481] = y[481]+__mv_accm468[0];;
  // stmt vectorized: y[481] += A[1195] * x[482];
  __mv_accm469 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1419 = _mm256_maskload_pd(&A[((i0 + 0) + 1196)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1420 = _mm256_maskload_pd(&x[((i0 + 0) + 481)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm469 = _mm256_fmadd_pd(__mv_vop1419, __mv_vop1420, __mv_accm469);	// latency = 5
    // stmt vectorized: y[0 * i0 + 482] += A[1 * i0 + 1196] * x[1 * i0 + 481];
  }
  __mv_vop1422 = _mm_loadu_pd(&A[1199]);	// latency = 1
  __mv_vop1423 = _mm_i64gather_pd(&x[482], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm470 = _mm_fmadd_pd(__mv_vop1422, __mv_vop1423, __mv_accm470);	// latency = 5
  // stmt vectorized: y[483] += A[1199] * x[482]
__mv_hi = _mm_shuffle_pd(__mv_accm470, __mv_accm470, 0x1);
__mv_accm470 = _mm_add_pd(__mv_accm470, __mv_hi);
y[483] = y[483]+__mv_accm470[0];;
  // stmt vectorized: y[483] += A[1200] * x[484];
  __mv_accm471 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1425 = _mm256_maskload_pd(&A[((i0 + 0) + 1201)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1426 = _mm256_maskload_pd(&x[((i0 + 0) + 483)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm471 = _mm256_fmadd_pd(__mv_vop1425, __mv_vop1426, __mv_accm471);	// latency = 5
    // stmt vectorized: y[0 * i0 + 484] += A[1 * i0 + 1201] * x[1 * i0 + 483];
  }
  __mv_vop1428 = _mm_loadu_pd(&A[1204]);	// latency = 1
  __mv_vop1429 = _mm_i64gather_pd(&x[484], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm472 = _mm_fmadd_pd(__mv_vop1428, __mv_vop1429, __mv_accm472);	// latency = 5
  // stmt vectorized: y[485] += A[1204] * x[484]
__mv_hi = _mm_shuffle_pd(__mv_accm472, __mv_accm472, 0x1);
__mv_accm472 = _mm_add_pd(__mv_accm472, __mv_hi);
y[485] = y[485]+__mv_accm472[0];;
  // stmt vectorized: y[485] += A[1205] * x[486];
  __mv_accm473 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1431 = _mm256_maskload_pd(&A[((i0 + 0) + 1206)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1432 = _mm256_maskload_pd(&x[((i0 + 0) + 485)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm473 = _mm256_fmadd_pd(__mv_vop1431, __mv_vop1432, __mv_accm473);	// latency = 5
    // stmt vectorized: y[0 * i0 + 486] += A[1 * i0 + 1206] * x[1 * i0 + 485];
  }__mv_accm467 = _mm256_hadd_pd(__mv_accm467, __mv_accm469);	// latency = 6
__mv_accm467 = _mm256_add_pd(_mm256_blend_pd(__mv_accm467,__mv_accm471,0b1100), _mm256_permute2f128_pd(__mv_accm467,__mv_accm471,0x21));	// latency = 4
y[480] = y[480] + __mv_accm467[0];	// latency = 1
y[482] = y[482] + __mv_accm467[1];	// latency = 1
y[484] = y[484] + __mv_accm467[2];	// latency = 1
y[486] = y[486] + __mv_accm467[3];	// latency = 1

  __mv_vop1434 = _mm_loadu_pd(&A[1209]);	// latency = 1
  __mv_vop1435 = _mm_i64gather_pd(&x[486], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm474 = _mm_fmadd_pd(__mv_vop1434, __mv_vop1435, __mv_accm474);	// latency = 5
  // stmt vectorized: y[487] += A[1209] * x[486]
__mv_hi = _mm_shuffle_pd(__mv_accm474, __mv_accm474, 0x1);
__mv_accm474 = _mm_add_pd(__mv_accm474, __mv_hi);
y[487] = y[487]+__mv_accm474[0];;
  // stmt vectorized: y[487] += A[1210] * x[488];
  __mv_accm475 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1437 = _mm256_maskload_pd(&A[((i0 + 0) + 1211)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1438 = _mm256_maskload_pd(&x[((i0 + 0) + 487)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm475 = _mm256_fmadd_pd(__mv_vop1437, __mv_vop1438, __mv_accm475);	// latency = 5
    // stmt vectorized: y[0 * i0 + 488] += A[1 * i0 + 1211] * x[1 * i0 + 487];
  }
  __mv_vop1440 = _mm_loadu_pd(&A[1214]);	// latency = 1
  __mv_vop1441 = _mm_i64gather_pd(&x[488], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm476 = _mm_fmadd_pd(__mv_vop1440, __mv_vop1441, __mv_accm476);	// latency = 5
  // stmt vectorized: y[489] += A[1214] * x[488]
__mv_hi = _mm_shuffle_pd(__mv_accm476, __mv_accm476, 0x1);
__mv_accm476 = _mm_add_pd(__mv_accm476, __mv_hi);
y[489] = y[489]+__mv_accm476[0];;
  // stmt vectorized: y[489] += A[1215] * x[490];
  __mv_accm477 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1443 = _mm256_maskload_pd(&A[((i0 + 0) + 1216)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1444 = _mm256_maskload_pd(&x[((i0 + 0) + 489)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm477 = _mm256_fmadd_pd(__mv_vop1443, __mv_vop1444, __mv_accm477);	// latency = 5
    // stmt vectorized: y[0 * i0 + 490] += A[1 * i0 + 1216] * x[1 * i0 + 489];
  }
  __mv_vop1446 = _mm_loadu_pd(&A[1219]);	// latency = 1
  __mv_vop1447 = _mm_i64gather_pd(&x[490], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm478 = _mm_fmadd_pd(__mv_vop1446, __mv_vop1447, __mv_accm478);	// latency = 5
  // stmt vectorized: y[491] += A[1219] * x[490]
__mv_hi = _mm_shuffle_pd(__mv_accm478, __mv_accm478, 0x1);
__mv_accm478 = _mm_add_pd(__mv_accm478, __mv_hi);
y[491] = y[491]+__mv_accm478[0];;
  // stmt vectorized: y[491] += A[1220] * x[492];
  __mv_accm479 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1449 = _mm256_maskload_pd(&A[((i0 + 0) + 1221)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1450 = _mm256_maskload_pd(&x[((i0 + 0) + 491)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm479 = _mm256_fmadd_pd(__mv_vop1449, __mv_vop1450, __mv_accm479);	// latency = 5
    // stmt vectorized: y[0 * i0 + 492] += A[1 * i0 + 1221] * x[1 * i0 + 491];
  }
  __mv_vop1452 = _mm_loadu_pd(&A[1224]);	// latency = 1
  __mv_vop1453 = _mm_i64gather_pd(&x[492], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm480 = _mm_fmadd_pd(__mv_vop1452, __mv_vop1453, __mv_accm480);	// latency = 5
  // stmt vectorized: y[493] += A[1224] * x[492]
__mv_hi = _mm_shuffle_pd(__mv_accm480, __mv_accm480, 0x1);
__mv_accm480 = _mm_add_pd(__mv_accm480, __mv_hi);
y[493] = y[493]+__mv_accm480[0];;
  // stmt vectorized: y[493] += A[1225] * x[494];
  __mv_accm481 = _mm_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop1455 = _mm_loadu_pd(&A[((i0 + 0) + 1226)]);	// latency = 1
    __mv_vop1456 = _mm_loadu_pd(&x[((i0 + 0) + 493)]);	// latency = 1
    __mv_accm481 = _mm_fmadd_pd(__mv_vop1455, __mv_vop1456, __mv_accm481);	// latency = 5
    // stmt vectorized: y[0 * i0 + 494] += A[1 * i0 + 1226] * x[1 * i0 + 493];
  }__mv_hi = _mm_shuffle_pd(__mv_accm481, __mv_accm481, 0x1);	// latency = 1
__mv_accm481 = _mm_add_pd(__mv_accm481, __mv_hi);	// latency = 4
y[494] = y[494]+__mv_accm481[0];	// latency = 1

  y[494] = (y[494] + (A[1228] * x[2251]));
  // stmt not vectorized: y[494] += A[1228] * x[2251];
  __mv_accm482 = _mm_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 1; i0 += 2) {
    __mv_vop1458 = _mm_loadu_pd(&A[((i0 + 0) + 1229)]);	// latency = 1
    __mv_vop1459 = _mm_loadu_pd(&x[((i0 + 0) + 495)]);	// latency = 1
    __mv_accm482 = _mm_fmadd_pd(__mv_vop1458, __mv_vop1459, __mv_accm482);	// latency = 5
    // stmt vectorized: y[0 * i0 + 495] += A[1 * i0 + 1229] * x[1 * i0 + 495];
  }__mv_hi = _mm_shuffle_pd(__mv_accm482, __mv_accm482, 0x1);	// latency = 1
__mv_accm482 = _mm_add_pd(__mv_accm482, __mv_hi);	// latency = 4
y[495] = y[495]+__mv_accm482[0];	// latency = 1

  y[495] = (y[495] + (A[1231] * x[1132]));
  // stmt not vectorized: y[495] += A[1231] * x[1132];
  __mv_vop1461 = _mm_loadu_pd(&A[1232]);	// latency = 1
  __mv_vop1462 = _mm_i64gather_pd(&x[495], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm483 = _mm_fmadd_pd(__mv_vop1461, __mv_vop1462, __mv_accm483);	// latency = 5
  // stmt vectorized: y[496] += A[1232] * x[495]
__mv_hi = _mm_shuffle_pd(__mv_accm483, __mv_accm483, 0x1);
__mv_accm483 = _mm_add_pd(__mv_accm483, __mv_hi);
y[496] = y[496]+__mv_accm483[0];;
  // stmt vectorized: y[496] += A[1233] * x[497];
  __mv_accm484 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1464 = _mm256_maskload_pd(&A[((i0 + 0) + 1234)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1465 = _mm256_maskload_pd(&x[((i0 + 0) + 496)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm484 = _mm256_fmadd_pd(__mv_vop1464, __mv_vop1465, __mv_accm484);	// latency = 5
    // stmt vectorized: y[0 * i0 + 497] += A[1 * i0 + 1234] * x[1 * i0 + 496];
  }__mv_accm475 = _mm256_hadd_pd(__mv_accm475, __mv_accm477);	// latency = 6
__mv_accm475 = _mm256_add_pd(_mm256_blend_pd(__mv_accm475,__mv_accm479,0b1100), _mm256_permute2f128_pd(__mv_accm475,__mv_accm479,0x21));	// latency = 4
y[488] = y[488] + __mv_accm475[0];	// latency = 1
y[490] = y[490] + __mv_accm475[1];	// latency = 1
y[492] = y[492] + __mv_accm475[2];	// latency = 1
y[497] = y[497] + __mv_accm475[3];	// latency = 1

  __mv_vop1467 = _mm_loadu_pd(&A[1237]);	// latency = 1
  __mv_vop1468 = _mm_i64gather_pd(&x[497], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm485 = _mm_fmadd_pd(__mv_vop1467, __mv_vop1468, __mv_accm485);	// latency = 5
  // stmt vectorized: y[498] += A[1237] * x[497]
__mv_hi = _mm_shuffle_pd(__mv_accm485, __mv_accm485, 0x1);
__mv_accm485 = _mm_add_pd(__mv_accm485, __mv_hi);
y[498] = y[498]+__mv_accm485[0];;
  // stmt vectorized: y[498] += A[1238] * x[499];
  __mv_accm486 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1470 = _mm256_maskload_pd(&A[((i0 + 0) + 1239)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1471 = _mm256_maskload_pd(&x[((i0 + 0) + 498)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm486 = _mm256_fmadd_pd(__mv_vop1470, __mv_vop1471, __mv_accm486);	// latency = 5
    // stmt vectorized: y[0 * i0 + 499] += A[1 * i0 + 1239] * x[1 * i0 + 498];
  }
  __mv_vop1473 = _mm_loadu_pd(&A[1242]);	// latency = 1
  __mv_vop1474 = _mm_i64gather_pd(&x[499], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm487 = _mm_fmadd_pd(__mv_vop1473, __mv_vop1474, __mv_accm487);	// latency = 5
  // stmt vectorized: y[500] += A[1242] * x[499]
__mv_hi = _mm_shuffle_pd(__mv_accm487, __mv_accm487, 0x1);
__mv_accm487 = _mm_add_pd(__mv_accm487, __mv_hi);
y[500] = y[500]+__mv_accm487[0];;
  // stmt vectorized: y[500] += A[1243] * x[501];
  __mv_accm488 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1476 = _mm256_maskload_pd(&A[((i0 + 0) + 1244)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1477 = _mm256_maskload_pd(&x[((i0 + 0) + 500)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm488 = _mm256_fmadd_pd(__mv_vop1476, __mv_vop1477, __mv_accm488);	// latency = 5
    // stmt vectorized: y[0 * i0 + 501] += A[1 * i0 + 1244] * x[1 * i0 + 500];
  }
  __mv_vop1479 = _mm_loadu_pd(&A[1247]);	// latency = 1
  __mv_vop1480 = _mm_i64gather_pd(&x[501], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm489 = _mm_fmadd_pd(__mv_vop1479, __mv_vop1480, __mv_accm489);	// latency = 5
  // stmt vectorized: y[502] += A[1247] * x[501]
__mv_hi = _mm_shuffle_pd(__mv_accm489, __mv_accm489, 0x1);
__mv_accm489 = _mm_add_pd(__mv_accm489, __mv_hi);
y[502] = y[502]+__mv_accm489[0];;
  // stmt vectorized: y[502] += A[1248] * x[503];
  __mv_accm490 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1482 = _mm256_maskload_pd(&A[((i0 + 0) + 1249)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1483 = _mm256_maskload_pd(&x[((i0 + 0) + 502)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm490 = _mm256_fmadd_pd(__mv_vop1482, __mv_vop1483, __mv_accm490);	// latency = 5
    // stmt vectorized: y[0 * i0 + 503] += A[1 * i0 + 1249] * x[1 * i0 + 502];
  }
  __mv_vop1485 = _mm_loadu_pd(&A[1252]);	// latency = 1
  __mv_vop1486 = _mm_i64gather_pd(&x[503], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm491 = _mm_fmadd_pd(__mv_vop1485, __mv_vop1486, __mv_accm491);	// latency = 5
  // stmt vectorized: y[504] += A[1252] * x[503]
__mv_hi = _mm_shuffle_pd(__mv_accm491, __mv_accm491, 0x1);
__mv_accm491 = _mm_add_pd(__mv_accm491, __mv_hi);
y[504] = y[504]+__mv_accm491[0];;
  // stmt vectorized: y[504] += A[1253] * x[505];
  __mv_accm492 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1488 = _mm256_maskload_pd(&A[((i0 + 0) + 1254)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1489 = _mm256_maskload_pd(&x[((i0 + 0) + 504)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm492 = _mm256_fmadd_pd(__mv_vop1488, __mv_vop1489, __mv_accm492);	// latency = 5
    // stmt vectorized: y[0 * i0 + 505] += A[1 * i0 + 1254] * x[1 * i0 + 504];
  }__mv_accm486 = _mm256_hadd_pd(__mv_accm486, __mv_accm488);	// latency = 6
__mv_accm486 = _mm256_add_pd(_mm256_blend_pd(__mv_accm486,__mv_accm490,0b1100), _mm256_permute2f128_pd(__mv_accm486,__mv_accm490,0x21));	// latency = 4
y[499] = y[499] + __mv_accm486[0];	// latency = 1
y[501] = y[501] + __mv_accm486[1];	// latency = 1
y[503] = y[503] + __mv_accm486[2];	// latency = 1
y[505] = y[505] + __mv_accm486[3];	// latency = 1

  __mv_vop1491 = _mm_loadu_pd(&A[1257]);	// latency = 1
  __mv_vop1492 = _mm_i64gather_pd(&x[505], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm493 = _mm_fmadd_pd(__mv_vop1491, __mv_vop1492, __mv_accm493);	// latency = 5
  // stmt vectorized: y[506] += A[1257] * x[505]
__mv_hi = _mm_shuffle_pd(__mv_accm493, __mv_accm493, 0x1);
__mv_accm493 = _mm_add_pd(__mv_accm493, __mv_hi);
y[506] = y[506]+__mv_accm493[0];;
  // stmt vectorized: y[506] += A[1258] * x[507];
  __mv_accm494 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1494 = _mm256_maskload_pd(&A[((i0 + 0) + 1259)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1495 = _mm256_maskload_pd(&x[((i0 + 0) + 506)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm494 = _mm256_fmadd_pd(__mv_vop1494, __mv_vop1495, __mv_accm494);	// latency = 5
    // stmt vectorized: y[0 * i0 + 507] += A[1 * i0 + 1259] * x[1 * i0 + 506];
  }
  __mv_vop1497 = _mm_loadu_pd(&A[1262]);	// latency = 1
  __mv_vop1498 = _mm_i64gather_pd(&x[507], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm495 = _mm_fmadd_pd(__mv_vop1497, __mv_vop1498, __mv_accm495);	// latency = 5
  // stmt vectorized: y[508] += A[1262] * x[507]
__mv_hi = _mm_shuffle_pd(__mv_accm495, __mv_accm495, 0x1);
__mv_accm495 = _mm_add_pd(__mv_accm495, __mv_hi);
y[508] = y[508]+__mv_accm495[0];;
  // stmt vectorized: y[508] += A[1263] * x[509];
  __mv_accm496 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1500 = _mm256_maskload_pd(&A[((i0 + 0) + 1264)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1501 = _mm256_maskload_pd(&x[((i0 + 0) + 508)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm496 = _mm256_fmadd_pd(__mv_vop1500, __mv_vop1501, __mv_accm496);	// latency = 5
    // stmt vectorized: y[0 * i0 + 509] += A[1 * i0 + 1264] * x[1 * i0 + 508];
  }
  __mv_vop1503 = _mm_loadu_pd(&A[1267]);	// latency = 1
  __mv_vop1504 = _mm_i64gather_pd(&x[509], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm497 = _mm_fmadd_pd(__mv_vop1503, __mv_vop1504, __mv_accm497);	// latency = 5
  // stmt vectorized: y[510] += A[1267] * x[509]
__mv_hi = _mm_shuffle_pd(__mv_accm497, __mv_accm497, 0x1);
__mv_accm497 = _mm_add_pd(__mv_accm497, __mv_hi);
y[510] = y[510]+__mv_accm497[0];;
  // stmt vectorized: y[510] += A[1268] * x[511];
  __mv_accm498 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1506 = _mm256_maskload_pd(&A[((i0 + 0) + 1269)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1507 = _mm256_maskload_pd(&x[((i0 + 0) + 510)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm498 = _mm256_fmadd_pd(__mv_vop1506, __mv_vop1507, __mv_accm498);	// latency = 5
    // stmt vectorized: y[0 * i0 + 511] += A[1 * i0 + 1269] * x[1 * i0 + 510];
  }
  __mv_vop1509 = _mm_loadu_pd(&A[1272]);	// latency = 1
  __mv_vop1510 = _mm_i64gather_pd(&x[511], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm499 = _mm_fmadd_pd(__mv_vop1509, __mv_vop1510, __mv_accm499);	// latency = 5
  // stmt vectorized: y[512] += A[1272] * x[511]
__mv_hi = _mm_shuffle_pd(__mv_accm499, __mv_accm499, 0x1);
__mv_accm499 = _mm_add_pd(__mv_accm499, __mv_hi);
y[512] = y[512]+__mv_accm499[0];;
  // stmt vectorized: y[512] += A[1273] * x[513];
  __mv_accm500 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1512 = _mm256_maskload_pd(&A[((i0 + 0) + 1274)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1513 = _mm256_maskload_pd(&x[((i0 + 0) + 512)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm500 = _mm256_fmadd_pd(__mv_vop1512, __mv_vop1513, __mv_accm500);	// latency = 5
    // stmt vectorized: y[0 * i0 + 513] += A[1 * i0 + 1274] * x[1 * i0 + 512];
  }__mv_accm494 = _mm256_hadd_pd(__mv_accm494, __mv_accm496);	// latency = 6
__mv_accm494 = _mm256_add_pd(_mm256_blend_pd(__mv_accm494,__mv_accm498,0b1100), _mm256_permute2f128_pd(__mv_accm494,__mv_accm498,0x21));	// latency = 4
y[507] = y[507] + __mv_accm494[0];	// latency = 1
y[509] = y[509] + __mv_accm494[1];	// latency = 1
y[511] = y[511] + __mv_accm494[2];	// latency = 1
y[513] = y[513] + __mv_accm494[3];	// latency = 1

  __mv_vop1515 = _mm_loadu_pd(&A[1277]);	// latency = 1
  __mv_vop1516 = _mm_i64gather_pd(&x[513], _mm_set_epi64x(2, 0), 8);	// latency = 10
  __mv_accm501 = _mm_fmadd_pd(__mv_vop1515, __mv_vop1516, __mv_accm501);	// latency = 5
  // stmt vectorized: y[514] += A[1277] * x[513]
__mv_hi = _mm_shuffle_pd(__mv_accm501, __mv_accm501, 0x1);
__mv_accm501 = _mm_add_pd(__mv_accm501, __mv_hi);
y[514] = y[514]+__mv_accm501[0];;
  // stmt vectorized: y[514] += A[1278] * x[515];
  __mv_accm502 = _mm256_setzero_pd();	// latency = 1
for (i0 = 0; i0 <= 2; i0 += 3) {
    __mv_vop1518 = _mm256_maskload_pd(&A[((i0 + 0) + 1279)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_vop1519 = _mm256_maskload_pd(&x[((i0 + 0) + 514)], _mm256_set_epi64x(0,-1,-1,-1));	// latency = 8
    __mv_accm502 = _mm256_fmadd_pd(__mv_vop1518, __mv_vop1519, __mv_accm502);	// latency = 5
    // stmt vectorized: y[0 * i0 + 515] += A[1 * i0 + 1279] * x[1 * i0 + 514];
  }__mv_lo256 = _mm256_castpd256_pd128(__mv_accm502);	// latency = 0
__mv_hi256 = _mm256_extractf128_pd(__mv_accm502, 0x1);	// latency = 3
__mv_lo256 = _mm_add_pd(__mv_lo256, __mv_hi256);	// latency = 4
__mv_hi256 = _mm_shuffle_pd(__mv_lo256, __mv_lo256, 0x1);	// latency = 1
__mv_lo256 = _mm_add_pd(__mv_lo256, __mv_hi256);	// latency = 4
y[515] = y[515]+__mv_lo256[0];	// latency = 1

  #pragma endmacveth
}

void kernel_spmv_fragment_1(double *restrict A, double *restrict x,
                            double *restrict y) {
  register int i0, i1, i2, i3, i4, i5, i6;

  y[516] += A[1282] * x[515];
  y[516] += A[1283] * x[517];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 517] += A[1 * i0 + 1284] * x[1 * i0 + 516];
  }
  y[518] += A[1287] * x[517];
  y[518] += A[1288] * x[519];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 519] += A[1 * i0 + 1289] * x[1 * i0 + 518];
  }
  y[520] += A[1292] * x[519];
  y[520] += A[1293] * x[521];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 521] += A[1 * i0 + 1294] * x[1 * i0 + 520];
  }
  y[522] += A[1297] * x[521];
  y[522] += A[1298] * x[523];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 523] += A[1 * i0 + 1299] * x[1 * i0 + 522];
  }
  y[524] += A[1302] * x[523];
  y[524] += A[1303] * x[525];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 525] += A[1 * i0 + 1304] * x[1 * i0 + 524];
  }
  y[526] += A[1307] * x[525];
  y[526] += A[1308] * x[527];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 527] += A[1 * i0 + 1309] * x[1 * i0 + 526];
  }
  y[528] += A[1312] * x[527];
  y[528] += A[1313] * x[529];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 529] += A[1 * i0 + 1314] * x[1 * i0 + 528];
  }
  y[530] += A[1317] * x[529];
  y[530] += A[1318] * x[531];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 531] += A[1 * i0 + 1319] * x[1 * i0 + 530];
  }
  y[532] += A[1322] * x[531];
  y[532] += A[1323] * x[533];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 533] += A[1 * i0 + 1324] * x[1 * i0 + 532];
  }
  y[534] += A[1327] * x[533];
  y[534] += A[1328] * x[535];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 535] += A[1 * i0 + 1329] * x[1 * i0 + 534];
  }
  y[536] += A[1332] * x[535];
  y[536] += A[1333] * x[537];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 537] += A[1 * i0 + 1334] * x[1 * i0 + 536];
  }
  y[538] += A[1337] * x[537];
  y[538] += A[1338] * x[539];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 539] += A[1 * i0 + 1339] * x[1 * i0 + 538];
  }
  y[540] += A[1342] * x[539];
  y[540] += A[1343] * x[541];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 541] += A[1 * i0 + 1344] * x[1 * i0 + 540];
  }
  y[542] += A[1347] * x[541];
  y[542] += A[1348] * x[543];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 543] += A[1 * i0 + 1349] * x[1 * i0 + 542];
  }
  y[544] += A[1352] * x[543];
  y[544] += A[1353] * x[545];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 545] += A[1 * i0 + 1354] * x[1 * i0 + 544];
  }
  y[546] += A[1357] * x[545];
  y[546] += A[1358] * x[547];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 547] += A[1 * i0 + 1359] * x[1 * i0 + 546];
  }
  y[548] += A[1362] * x[547];
  y[548] += A[1363] * x[549];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 549] += A[1 * i0 + 1364] * x[1 * i0 + 548];
  }
  y[550] += A[1367] * x[549];
  y[550] += A[1368] * x[551];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 551] += A[1 * i0 + 1369] * x[1 * i0 + 550];
  }
  y[552] += A[1372] * x[551];
  y[552] += A[1373] * x[553];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 553] += A[1 * i0 + 1374] * x[1 * i0 + 552];
  }
  y[554] += A[1377] * x[553];
  y[554] += A[1378] * x[555];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 555] += A[1 * i0 + 1379] * x[1 * i0 + 554];
  }
  y[556] += A[1382] * x[555];
  y[556] += A[1383] * x[557];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 557] += A[1 * i0 + 1384] * x[1 * i0 + 556];
  }
  y[558] += A[1387] * x[557];
  y[558] += A[1388] * x[559];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 559] += A[1 * i0 + 1389] * x[1 * i0 + 558];
  }
  y[560] += A[1392] * x[559];
  y[560] += A[1393] * x[561];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 561] += A[1 * i0 + 1394] * x[1 * i0 + 560];
  }
  y[562] += A[1397] * x[561];
  y[562] += A[1398] * x[563];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 563] += A[1 * i0 + 1399] * x[1 * i0 + 562];
  }
  y[564] += A[1402] * x[563];
  y[564] += A[1403] * x[565];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 565] += A[1 * i0 + 1404] * x[1 * i0 + 564];
  }
  y[566] += A[1407] * x[565];
  y[566] += A[1408] * x[567];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 567] += A[1 * i0 + 1409] * x[1 * i0 + 566];
  }
  y[568] += A[1412] * x[567];
  y[568] += A[1413] * x[569];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 569] += A[1 * i0 + 1414] * x[1 * i0 + 568];
  }
  y[570] += A[1417] * x[569];
  y[570] += A[1418] * x[571];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 571] += A[1 * i0 + 1419] * x[1 * i0 + 570];
  }
  y[572] += A[1422] * x[571];
  y[572] += A[1423] * x[573];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 573] += A[1 * i0 + 1424] * x[1 * i0 + 572];
  }
  y[574] += A[1427] * x[573];
  y[574] += A[1428] * x[575];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 575] += A[1 * i0 + 1429] * x[1 * i0 + 574];
  }
  y[576] += A[1432] * x[575];
  y[576] += A[1433] * x[577];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 577] += A[1 * i0 + 1434] * x[1 * i0 + 576];
  }
  y[578] += A[1437] * x[577];
  y[578] += A[1438] * x[579];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 579] += A[1 * i0 + 1439] * x[1 * i0 + 578];
  }
  y[580] += A[1442] * x[579];
  y[580] += A[1443] * x[581];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 581] += A[1 * i0 + 1444] * x[1 * i0 + 580];
  }
  y[582] += A[1447] * x[581];
  y[582] += A[1448] * x[583];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 583] += A[1 * i0 + 1449] * x[1 * i0 + 582];
  }
  y[584] += A[1452] * x[583];
  y[584] += A[1453] * x[585];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 585] += A[1 * i0 + 1454] * x[1 * i0 + 584];
  }
  y[586] += A[1457] * x[585];
  y[586] += A[1458] * x[587];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 587] += A[1 * i0 + 1459] * x[1 * i0 + 586];
  }
  y[588] += A[1462] * x[587];
  y[588] += A[1463] * x[589];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 589] += A[1 * i0 + 1464] * x[1 * i0 + 588];
  }
  y[590] += A[1467] * x[589];
  y[590] += A[1468] * x[591];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 591] += A[1 * i0 + 1469] * x[1 * i0 + 590];
  }
  y[592] += A[1472] * x[591];
  y[592] += A[1473] * x[593];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 593] += A[1 * i0 + 1474] * x[1 * i0 + 592];
  }
  y[594] += A[1477] * x[593];
  y[594] += A[1478] * x[595];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 595] += A[1 * i0 + 1479] * x[1 * i0 + 594];
  }
  y[596] += A[1482] * x[595];
  y[596] += A[1483] * x[597];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 597] += A[1 * i0 + 1484] * x[1 * i0 + 596];
  }
  y[598] += A[1487] * x[597];
  y[598] += A[1488] * x[599];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 599] += A[1 * i0 + 1489] * x[1 * i0 + 598];
  }
  y[600] += A[1492] * x[599];
  y[600] += A[1493] * x[601];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 601] += A[1 * i0 + 1494] * x[1 * i0 + 600];
  }
  y[602] += A[1497] * x[601];
  y[602] += A[1498] * x[603];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 603] += A[1 * i0 + 1499] * x[1 * i0 + 602];
  }
  y[604] += A[1502] * x[603];
  y[604] += A[1503] * x[605];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 605] += A[1 * i0 + 1504] * x[1 * i0 + 604];
  }
  y[606] += A[1507] * x[605];
  y[606] += A[1508] * x[607];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 607] += A[1 * i0 + 1509] * x[1 * i0 + 606];
  }
  y[608] += A[1512] * x[607];
  y[608] += A[1513] * x[609];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 609] += A[1 * i0 + 1514] * x[1 * i0 + 608];
  }
  y[610] += A[1517] * x[609];
  y[610] += A[1518] * x[611];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 611] += A[1 * i0 + 1519] * x[1 * i0 + 610];
  }
  y[612] += A[1522] * x[611];
  y[612] += A[1523] * x[613];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 613] += A[1 * i0 + 1524] * x[1 * i0 + 612];
  }
  y[614] += A[1527] * x[613];
  y[614] += A[1528] * x[615];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 615] += A[1 * i0 + 1529] * x[1 * i0 + 614];
  }
  y[616] += A[1532] * x[615];
  y[616] += A[1533] * x[617];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 617] += A[1 * i0 + 1534] * x[1 * i0 + 616];
  }
  y[618] += A[1537] * x[617];
  y[618] += A[1538] * x[619];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 619] += A[1 * i0 + 1539] * x[1 * i0 + 618];
  }
  y[620] += A[1542] * x[619];
  y[620] += A[1543] * x[621];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 621] += A[1 * i0 + 1544] * x[1 * i0 + 620];
  }
  y[622] += A[1547] * x[621];
  y[622] += A[1548] * x[623];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 623] += A[1 * i0 + 1549] * x[1 * i0 + 622];
  }
  y[624] += A[1552] * x[623];
  y[624] += A[1553] * x[625];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 625] += A[1 * i0 + 1554] * x[1 * i0 + 624];
  }
  y[626] += A[1557] * x[625];
  y[626] += A[1558] * x[627];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 627] += A[1 * i0 + 1559] * x[1 * i0 + 626];
  }
  y[628] += A[1562] * x[627];
  y[628] += A[1563] * x[629];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 629] += A[1 * i0 + 1564] * x[1 * i0 + 628];
  }
  y[630] += A[1567] * x[629];
  y[630] += A[1568] * x[631];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 631] += A[1 * i0 + 1569] * x[1 * i0 + 630];
  }
  y[632] += A[1572] * x[631];
  y[632] += A[1573] * x[633];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 633] += A[1 * i0 + 1574] * x[1 * i0 + 632];
  }
  y[634] += A[1577] * x[633];
  y[634] += A[1578] * x[635];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 635] += A[1 * i0 + 1579] * x[1 * i0 + 634];
  }
  y[636] += A[1582] * x[635];
  y[636] += A[1583] * x[637];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 637] += A[1 * i0 + 1584] * x[1 * i0 + 636];
  }
  y[638] += A[1587] * x[637];
  y[638] += A[1588] * x[639];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 639] += A[1 * i0 + 1589] * x[1 * i0 + 638];
  }
  y[640] += A[1592] * x[639];
  y[640] += A[1593] * x[641];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 641] += A[1 * i0 + 1594] * x[1 * i0 + 640];
  }
  y[642] += A[1597] * x[641];
  y[642] += A[1598] * x[643];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 643] += A[1 * i0 + 1599] * x[1 * i0 + 642];
  }
  y[644] += A[1602] * x[643];
  y[644] += A[1603] * x[645];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 645] += A[1 * i0 + 1604] * x[1 * i0 + 644];
  }
  y[646] += A[1607] * x[645];
  y[646] += A[1608] * x[647];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 647] += A[1 * i0 + 1609] * x[1 * i0 + 646];
  }
  y[648] += A[1612] * x[647];
  y[648] += A[1613] * x[649];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 649] += A[1 * i0 + 1614] * x[1 * i0 + 648];
  }
  y[650] += A[1617] * x[649];
  y[650] += A[1618] * x[651];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 651] += A[1 * i0 + 1619] * x[1 * i0 + 650];
  }
  y[652] += A[1622] * x[651];
  y[652] += A[1623] * x[653];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 653] += A[1 * i0 + 1624] * x[1 * i0 + 652];
  }
  y[653] += A[1626] * x[1612];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 654] += A[1 * i0 + 1627] * x[1 * i0 + 654];
  }
  y[654] += A[1629] * x[813];
  y[655] += A[1630] * x[654];
  y[655] += A[1631] * x[656];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 656] += A[1 * i0 + 1632] * x[1 * i0 + 655];
  }
  y[657] += A[1635] * x[656];
  y[657] += A[1636] * x[658];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 658] += A[1 * i0 + 1637] * x[1 * i0 + 657];
  }
  y[659] += A[1640] * x[658];
  y[659] += A[1641] * x[660];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 660] += A[1 * i0 + 1642] * x[1 * i0 + 659];
  }
  y[661] += A[1645] * x[660];
  y[661] += A[1646] * x[662];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 662] += A[1 * i0 + 1647] * x[1 * i0 + 661];
  }
  y[663] += A[1650] * x[662];
  y[663] += A[1651] * x[664];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 664] += A[1 * i0 + 1652] * x[1 * i0 + 663];
  }
  y[665] += A[1655] * x[664];
  y[665] += A[1656] * x[666];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 666] += A[1 * i0 + 1657] * x[1 * i0 + 665];
  }
  y[667] += A[1660] * x[666];
  y[667] += A[1661] * x[668];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 668] += A[1 * i0 + 1662] * x[1 * i0 + 667];
  }
  y[669] += A[1665] * x[668];
  y[669] += A[1666] * x[670];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 670] += A[1 * i0 + 1667] * x[1 * i0 + 669];
  }
  y[671] += A[1670] * x[670];
  y[671] += A[1671] * x[672];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 672] += A[1 * i0 + 1672] * x[1 * i0 + 671];
  }
  y[673] += A[1675] * x[672];
  y[673] += A[1676] * x[674];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 674] += A[1 * i0 + 1677] * x[1 * i0 + 673];
  }
  y[675] += A[1680] * x[674];
  y[675] += A[1681] * x[676];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 676] += A[1 * i0 + 1682] * x[1 * i0 + 675];
  }
  y[677] += A[1685] * x[676];
  y[677] += A[1686] * x[678];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 678] += A[1 * i0 + 1687] * x[1 * i0 + 677];
  }
  y[679] += A[1690] * x[678];
  y[679] += A[1691] * x[680];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 680] += A[1 * i0 + 1692] * x[1 * i0 + 679];
  }
  y[681] += A[1695] * x[680];
  y[681] += A[1696] * x[682];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 682] += A[1 * i0 + 1697] * x[1 * i0 + 681];
  }
  y[683] += A[1700] * x[682];
  y[683] += A[1701] * x[684];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 684] += A[1 * i0 + 1702] * x[1 * i0 + 683];
  }
  y[685] += A[1705] * x[684];
  y[685] += A[1706] * x[686];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 686] += A[1 * i0 + 1707] * x[1 * i0 + 685];
  }
  y[687] += A[1710] * x[686];
  y[687] += A[1711] * x[688];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 688] += A[1 * i0 + 1712] * x[1 * i0 + 687];
  }
  y[689] += A[1715] * x[688];
  y[689] += A[1716] * x[690];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 690] += A[1 * i0 + 1717] * x[1 * i0 + 689];
  }
  y[691] += A[1720] * x[690];
  y[691] += A[1721] * x[692];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 692] += A[1 * i0 + 1722] * x[1 * i0 + 691];
  }
  y[693] += A[1725] * x[692];
  y[693] += A[1726] * x[694];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 694] += A[1 * i0 + 1727] * x[1 * i0 + 693];
  }
  y[695] += A[1730] * x[694];
  y[695] += A[1731] * x[696];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 696] += A[1 * i0 + 1732] * x[1 * i0 + 695];
  }
  y[697] += A[1735] * x[696];
  y[697] += A[1736] * x[698];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 698] += A[1 * i0 + 1737] * x[1 * i0 + 697];
  }
  y[699] += A[1740] * x[698];
  y[699] += A[1741] * x[700];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 700] += A[1 * i0 + 1742] * x[1 * i0 + 699];
  }
  y[701] += A[1745] * x[700];
  y[701] += A[1746] * x[702];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 702] += A[1 * i0 + 1747] * x[1 * i0 + 701];
  }
  y[703] += A[1750] * x[702];
  y[703] += A[1751] * x[704];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 704] += A[1 * i0 + 1752] * x[1 * i0 + 703];
  }
  y[705] += A[1755] * x[704];
  y[705] += A[1756] * x[706];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 706] += A[1 * i0 + 1757] * x[1 * i0 + 705];
  }
  y[707] += A[1760] * x[706];
  y[707] += A[1761] * x[708];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 708] += A[1 * i0 + 1762] * x[1 * i0 + 707];
  }
  y[709] += A[1765] * x[708];
  y[709] += A[1766] * x[710];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 710] += A[1 * i0 + 1767] * x[1 * i0 + 709];
  }
  y[711] += A[1770] * x[710];
  y[711] += A[1771] * x[712];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 712] += A[1 * i0 + 1772] * x[1 * i0 + 711];
  }
  y[713] += A[1775] * x[712];
  y[713] += A[1776] * x[714];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 714] += A[1 * i0 + 1777] * x[1 * i0 + 713];
  }
  y[715] += A[1780] * x[714];
  y[715] += A[1781] * x[716];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 716] += A[1 * i0 + 1782] * x[1 * i0 + 715];
  }
  y[717] += A[1785] * x[716];
  y[717] += A[1786] * x[718];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 718] += A[1 * i0 + 1787] * x[1 * i0 + 717];
  }
  y[719] += A[1790] * x[718];
  y[719] += A[1791] * x[720];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 720] += A[1 * i0 + 1792] * x[1 * i0 + 719];
  }
  y[721] += A[1795] * x[720];
  y[721] += A[1796] * x[722];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 722] += A[1 * i0 + 1797] * x[1 * i0 + 721];
  }
  y[723] += A[1800] * x[722];
  y[723] += A[1801] * x[724];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 724] += A[1 * i0 + 1802] * x[1 * i0 + 723];
  }
  y[725] += A[1805] * x[724];
  y[725] += A[1806] * x[726];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 726] += A[1 * i0 + 1807] * x[1 * i0 + 725];
  }
  y[727] += A[1810] * x[726];
  y[727] += A[1811] * x[728];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 728] += A[1 * i0 + 1812] * x[1 * i0 + 727];
  }
  y[729] += A[1815] * x[728];
  y[729] += A[1816] * x[730];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 730] += A[1 * i0 + 1817] * x[1 * i0 + 729];
  }
  y[731] += A[1820] * x[730];
  y[731] += A[1821] * x[732];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 732] += A[1 * i0 + 1822] * x[1 * i0 + 731];
  }
  y[733] += A[1825] * x[732];
  y[733] += A[1826] * x[734];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 734] += A[1 * i0 + 1827] * x[1 * i0 + 733];
  }
  y[735] += A[1830] * x[734];
  y[735] += A[1831] * x[736];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 736] += A[1 * i0 + 1832] * x[1 * i0 + 735];
  }
  y[737] += A[1835] * x[736];
  y[737] += A[1836] * x[738];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 738] += A[1 * i0 + 1837] * x[1 * i0 + 737];
  }
  y[739] += A[1840] * x[738];
  y[739] += A[1841] * x[740];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 740] += A[1 * i0 + 1842] * x[1 * i0 + 739];
  }
  y[741] += A[1845] * x[740];
  y[741] += A[1846] * x[742];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 742] += A[1 * i0 + 1847] * x[1 * i0 + 741];
  }
  y[743] += A[1850] * x[742];
  y[743] += A[1851] * x[744];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 744] += A[1 * i0 + 1852] * x[1 * i0 + 743];
  }
  y[745] += A[1855] * x[744];
  y[745] += A[1856] * x[746];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 746] += A[1 * i0 + 1857] * x[1 * i0 + 745];
  }
  y[747] += A[1860] * x[746];
  y[747] += A[1861] * x[748];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 748] += A[1 * i0 + 1862] * x[1 * i0 + 747];
  }
  y[749] += A[1865] * x[748];
  y[749] += A[1866] * x[750];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 750] += A[1 * i0 + 1867] * x[1 * i0 + 749];
  }
  y[751] += A[1870] * x[750];
  y[751] += A[1871] * x[752];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 752] += A[1 * i0 + 1872] * x[1 * i0 + 751];
  }
  y[753] += A[1875] * x[752];
  y[753] += A[1876] * x[754];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 754] += A[1 * i0 + 1877] * x[1 * i0 + 753];
  }
  y[755] += A[1880] * x[754];
  y[755] += A[1881] * x[756];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 756] += A[1 * i0 + 1882] * x[1 * i0 + 755];
  }
  y[757] += A[1885] * x[756];
  y[757] += A[1886] * x[758];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 758] += A[1 * i0 + 1887] * x[1 * i0 + 757];
  }
  y[759] += A[1890] * x[758];
  y[759] += A[1891] * x[760];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 760] += A[1 * i0 + 1892] * x[1 * i0 + 759];
  }
  y[761] += A[1895] * x[760];
  y[761] += A[1896] * x[762];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 762] += A[1 * i0 + 1897] * x[1 * i0 + 761];
  }
  y[763] += A[1900] * x[762];
  y[763] += A[1901] * x[764];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 764] += A[1 * i0 + 1902] * x[1 * i0 + 763];
  }
  y[765] += A[1905] * x[764];
  y[765] += A[1906] * x[766];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 766] += A[1 * i0 + 1907] * x[1 * i0 + 765];
  }
  y[767] += A[1910] * x[766];
  y[767] += A[1911] * x[768];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 768] += A[1 * i0 + 1912] * x[1 * i0 + 767];
  }
  y[769] += A[1915] * x[768];
  y[769] += A[1916] * x[770];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 770] += A[1 * i0 + 1917] * x[1 * i0 + 769];
  }
  y[771] += A[1920] * x[770];
  y[771] += A[1921] * x[772];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 772] += A[1 * i0 + 1922] * x[1 * i0 + 771];
  }
  y[773] += A[1925] * x[772];
  y[773] += A[1926] * x[774];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 774] += A[1 * i0 + 1927] * x[1 * i0 + 773];
  }
  y[775] += A[1930] * x[774];
  y[775] += A[1931] * x[776];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 776] += A[1 * i0 + 1932] * x[1 * i0 + 775];
  }
  y[777] += A[1935] * x[776];
  y[777] += A[1936] * x[778];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 778] += A[1 * i0 + 1937] * x[1 * i0 + 777];
  }
  y[779] += A[1940] * x[778];
  y[779] += A[1941] * x[780];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 780] += A[1 * i0 + 1942] * x[1 * i0 + 779];
  }
  y[781] += A[1945] * x[780];
  y[781] += A[1946] * x[782];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 782] += A[1 * i0 + 1947] * x[1 * i0 + 781];
  }
  y[783] += A[1950] * x[782];
  y[783] += A[1951] * x[784];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 784] += A[1 * i0 + 1952] * x[1 * i0 + 783];
  }
  y[785] += A[1955] * x[784];
  y[785] += A[1956] * x[786];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 786] += A[1 * i0 + 1957] * x[1 * i0 + 785];
  }
  y[787] += A[1960] * x[786];
  y[787] += A[1961] * x[788];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 788] += A[1 * i0 + 1962] * x[1 * i0 + 787];
  }
  y[789] += A[1965] * x[788];
  y[789] += A[1966] * x[790];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 790] += A[1 * i0 + 1967] * x[1 * i0 + 789];
  }
  y[791] += A[1970] * x[790];
  y[791] += A[1971] * x[792];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 792] += A[1 * i0 + 1972] * x[1 * i0 + 791];
  }
  y[793] += A[1975] * x[792];
  y[793] += A[1976] * x[794];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 794] += A[1 * i0 + 1977] * x[1 * i0 + 793];
  }
  y[795] += A[1980] * x[794];
  y[795] += A[1981] * x[796];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 796] += A[1 * i0 + 1982] * x[1 * i0 + 795];
  }
  y[797] += A[1985] * x[796];
  y[797] += A[1986] * x[798];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 798] += A[1 * i0 + 1987] * x[1 * i0 + 797];
  }
  y[799] += A[1990] * x[798];
  y[799] += A[1991] * x[800];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 800] += A[1 * i0 + 1992] * x[1 * i0 + 799];
  }
  y[801] += A[1995] * x[800];
  y[801] += A[1996] * x[802];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 802] += A[1 * i0 + 1997] * x[1 * i0 + 801];
  }
  y[803] += A[2000] * x[802];
  y[803] += A[2001] * x[804];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 804] += A[1 * i0 + 2002] * x[1 * i0 + 803];
  }
  y[805] += A[2005] * x[804];
  y[805] += A[2006] * x[806];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 806] += A[1 * i0 + 2007] * x[1 * i0 + 805];
  }
  y[807] += A[2010] * x[806];
  y[807] += A[2011] * x[808];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 808] += A[1 * i0 + 2012] * x[1 * i0 + 807];
  }
  y[809] += A[2015] * x[808];
  y[809] += A[2016] * x[810];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 810] += A[1 * i0 + 2017] * x[1 * i0 + 809];
  }
  y[811] += A[2020] * x[810];
  y[811] += A[2021] * x[812];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 812] += A[1 * i0 + 2022] * x[1 * i0 + 811];
  }
  y[812] += A[2024] * x[1612];
  y[813] += A[2025] * x[654];
  y[813] += A[2026] * x[814];
  y[813] += A[2027] * x[4017];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 814] += A[1 * i0 + 2028] * x[1 * i0 + 813];
  }
  y[815] += A[2031] * x[814];
  y[815] += A[2032] * x[816];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 816] += A[1 * i0 + 2033] * x[1 * i0 + 815];
  }
  y[817] += A[2036] * x[816];
  y[817] += A[2037] * x[818];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 818] += A[1 * i0 + 2038] * x[1 * i0 + 817];
  }
  y[819] += A[2041] * x[818];
  y[819] += A[2042] * x[820];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 820] += A[1 * i0 + 2043] * x[1 * i0 + 819];
  }
  y[821] += A[2046] * x[820];
  y[821] += A[2047] * x[822];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 822] += A[1 * i0 + 2048] * x[1 * i0 + 821];
  }
  y[823] += A[2051] * x[822];
  y[823] += A[2052] * x[824];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 824] += A[1 * i0 + 2053] * x[1 * i0 + 823];
  }
  y[825] += A[2056] * x[824];
  y[825] += A[2057] * x[826];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 826] += A[1 * i0 + 2058] * x[1 * i0 + 825];
  }
  y[827] += A[2061] * x[826];
  y[827] += A[2062] * x[828];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 828] += A[1 * i0 + 2063] * x[1 * i0 + 827];
  }
  y[829] += A[2066] * x[828];
  y[829] += A[2067] * x[830];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 830] += A[1 * i0 + 2068] * x[1 * i0 + 829];
  }
  y[831] += A[2071] * x[830];
  y[831] += A[2072] * x[832];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 832] += A[1 * i0 + 2073] * x[1 * i0 + 831];
  }
  y[833] += A[2076] * x[832];
  y[833] += A[2077] * x[834];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 834] += A[1 * i0 + 2078] * x[1 * i0 + 833];
  }
  y[835] += A[2081] * x[834];
  y[835] += A[2082] * x[836];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 836] += A[1 * i0 + 2083] * x[1 * i0 + 835];
  }
  y[837] += A[2086] * x[836];
  y[837] += A[2087] * x[838];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 838] += A[1 * i0 + 2088] * x[1 * i0 + 837];
  }
  y[839] += A[2091] * x[838];
  y[839] += A[2092] * x[840];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 840] += A[1 * i0 + 2093] * x[1 * i0 + 839];
  }
  y[841] += A[2096] * x[840];
  y[841] += A[2097] * x[842];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 842] += A[1 * i0 + 2098] * x[1 * i0 + 841];
  }
  y[843] += A[2101] * x[842];
  y[843] += A[2102] * x[844];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 844] += A[1 * i0 + 2103] * x[1 * i0 + 843];
  }
  y[845] += A[2106] * x[844];
  y[845] += A[2107] * x[846];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 846] += A[1 * i0 + 2108] * x[1 * i0 + 845];
  }
  y[847] += A[2111] * x[846];
  y[847] += A[2112] * x[848];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 848] += A[1 * i0 + 2113] * x[1 * i0 + 847];
  }
  y[849] += A[2116] * x[848];
  y[849] += A[2117] * x[850];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 850] += A[1 * i0 + 2118] * x[1 * i0 + 849];
  }
  y[851] += A[2121] * x[850];
  y[851] += A[2122] * x[852];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 852] += A[1 * i0 + 2123] * x[1 * i0 + 851];
  }
  y[853] += A[2126] * x[852];
  y[853] += A[2127] * x[854];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 854] += A[1 * i0 + 2128] * x[1 * i0 + 853];
  }
  y[855] += A[2131] * x[854];
  y[855] += A[2132] * x[856];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 856] += A[1 * i0 + 2133] * x[1 * i0 + 855];
  }
  y[857] += A[2136] * x[856];
  y[857] += A[2137] * x[858];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 858] += A[1 * i0 + 2138] * x[1 * i0 + 857];
  }
  y[859] += A[2141] * x[858];
  y[859] += A[2142] * x[860];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 860] += A[1 * i0 + 2143] * x[1 * i0 + 859];
  }
  y[861] += A[2146] * x[860];
  y[861] += A[2147] * x[862];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 862] += A[1 * i0 + 2148] * x[1 * i0 + 861];
  }
  y[863] += A[2151] * x[862];
  y[863] += A[2152] * x[864];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 864] += A[1 * i0 + 2153] * x[1 * i0 + 863];
  }
  y[865] += A[2156] * x[864];
  y[865] += A[2157] * x[866];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 866] += A[1 * i0 + 2158] * x[1 * i0 + 865];
  }
  y[867] += A[2161] * x[866];
  y[867] += A[2162] * x[868];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 868] += A[1 * i0 + 2163] * x[1 * i0 + 867];
  }
  y[869] += A[2166] * x[868];
  y[869] += A[2167] * x[870];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 870] += A[1 * i0 + 2168] * x[1 * i0 + 869];
  }
  y[871] += A[2171] * x[870];
  y[871] += A[2172] * x[872];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 872] += A[1 * i0 + 2173] * x[1 * i0 + 871];
  }
  y[873] += A[2176] * x[872];
  y[873] += A[2177] * x[874];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 874] += A[1 * i0 + 2178] * x[1 * i0 + 873];
  }
  y[875] += A[2181] * x[874];
  y[875] += A[2182] * x[876];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 876] += A[1 * i0 + 2183] * x[1 * i0 + 875];
  }
  y[877] += A[2186] * x[876];
  y[877] += A[2187] * x[878];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 878] += A[1 * i0 + 2188] * x[1 * i0 + 877];
  }
  y[879] += A[2191] * x[878];
  y[879] += A[2192] * x[880];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 880] += A[1 * i0 + 2193] * x[1 * i0 + 879];
  }
  y[881] += A[2196] * x[880];
  y[881] += A[2197] * x[882];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 882] += A[1 * i0 + 2198] * x[1 * i0 + 881];
  }
  y[883] += A[2201] * x[882];
  y[883] += A[2202] * x[884];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 884] += A[1 * i0 + 2203] * x[1 * i0 + 883];
  }
  y[885] += A[2206] * x[884];
  y[885] += A[2207] * x[886];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 886] += A[1 * i0 + 2208] * x[1 * i0 + 885];
  }
  y[887] += A[2211] * x[886];
  y[887] += A[2212] * x[888];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 888] += A[1 * i0 + 2213] * x[1 * i0 + 887];
  }
  y[889] += A[2216] * x[888];
  y[889] += A[2217] * x[890];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 890] += A[1 * i0 + 2218] * x[1 * i0 + 889];
  }
  y[891] += A[2221] * x[890];
  y[891] += A[2222] * x[892];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 892] += A[1 * i0 + 2223] * x[1 * i0 + 891];
  }
  y[893] += A[2226] * x[892];
  y[893] += A[2227] * x[894];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 894] += A[1 * i0 + 2228] * x[1 * i0 + 893];
  }
  y[895] += A[2231] * x[894];
  y[895] += A[2232] * x[896];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 896] += A[1 * i0 + 2233] * x[1 * i0 + 895];
  }
  y[897] += A[2236] * x[896];
  y[897] += A[2237] * x[898];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 898] += A[1 * i0 + 2238] * x[1 * i0 + 897];
  }
  y[899] += A[2241] * x[898];
  y[899] += A[2242] * x[900];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 900] += A[1 * i0 + 2243] * x[1 * i0 + 899];
  }
  y[901] += A[2246] * x[900];
  y[901] += A[2247] * x[902];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 902] += A[1 * i0 + 2248] * x[1 * i0 + 901];
  }
  y[903] += A[2251] * x[902];
  y[903] += A[2252] * x[904];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 904] += A[1 * i0 + 2253] * x[1 * i0 + 903];
  }
  y[905] += A[2256] * x[904];
  y[905] += A[2257] * x[906];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 906] += A[1 * i0 + 2258] * x[1 * i0 + 905];
  }
  y[907] += A[2261] * x[906];
  y[907] += A[2262] * x[908];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 908] += A[1 * i0 + 2263] * x[1 * i0 + 907];
  }
  y[909] += A[2266] * x[908];
  y[909] += A[2267] * x[910];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 910] += A[1 * i0 + 2268] * x[1 * i0 + 909];
  }
  y[911] += A[2271] * x[910];
  y[911] += A[2272] * x[912];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 912] += A[1 * i0 + 2273] * x[1 * i0 + 911];
  }
  y[913] += A[2276] * x[912];
  y[913] += A[2277] * x[914];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 914] += A[1 * i0 + 2278] * x[1 * i0 + 913];
  }
  y[915] += A[2281] * x[914];
  y[915] += A[2282] * x[916];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 916] += A[1 * i0 + 2283] * x[1 * i0 + 915];
  }
  y[917] += A[2286] * x[916];
  y[917] += A[2287] * x[918];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 918] += A[1 * i0 + 2288] * x[1 * i0 + 917];
  }
  y[919] += A[2291] * x[918];
  y[919] += A[2292] * x[920];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 920] += A[1 * i0 + 2293] * x[1 * i0 + 919];
  }
  y[921] += A[2296] * x[920];
  y[921] += A[2297] * x[922];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 922] += A[1 * i0 + 2298] * x[1 * i0 + 921];
  }
  y[923] += A[2301] * x[922];
  y[923] += A[2302] * x[924];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 924] += A[1 * i0 + 2303] * x[1 * i0 + 923];
  }
  y[925] += A[2306] * x[924];
  y[925] += A[2307] * x[926];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 926] += A[1 * i0 + 2308] * x[1 * i0 + 925];
  }
  y[927] += A[2311] * x[926];
  y[927] += A[2312] * x[928];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 928] += A[1 * i0 + 2313] * x[1 * i0 + 927];
  }
  y[929] += A[2316] * x[928];
  y[929] += A[2317] * x[930];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 930] += A[1 * i0 + 2318] * x[1 * i0 + 929];
  }
  y[931] += A[2321] * x[930];
  y[931] += A[2322] * x[932];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 932] += A[1 * i0 + 2323] * x[1 * i0 + 931];
  }
  y[933] += A[2326] * x[932];
  y[933] += A[2327] * x[934];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 934] += A[1 * i0 + 2328] * x[1 * i0 + 933];
  }
  y[935] += A[2331] * x[934];
  y[935] += A[2332] * x[936];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 936] += A[1 * i0 + 2333] * x[1 * i0 + 935];
  }
  y[937] += A[2336] * x[936];
  y[937] += A[2337] * x[938];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 938] += A[1 * i0 + 2338] * x[1 * i0 + 937];
  }
  y[939] += A[2341] * x[938];
  y[939] += A[2342] * x[940];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 940] += A[1 * i0 + 2343] * x[1 * i0 + 939];
  }
  y[941] += A[2346] * x[940];
  y[941] += A[2347] * x[942];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 942] += A[1 * i0 + 2348] * x[1 * i0 + 941];
  }
  y[943] += A[2351] * x[942];
  y[943] += A[2352] * x[944];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 944] += A[1 * i0 + 2353] * x[1 * i0 + 943];
  }
  y[945] += A[2356] * x[944];
  y[945] += A[2357] * x[946];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 946] += A[1 * i0 + 2358] * x[1 * i0 + 945];
  }
  y[947] += A[2361] * x[946];
  y[947] += A[2362] * x[948];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 948] += A[1 * i0 + 2363] * x[1 * i0 + 947];
  }
  y[949] += A[2366] * x[948];
  y[949] += A[2367] * x[950];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 950] += A[1 * i0 + 2368] * x[1 * i0 + 949];
  }
  y[951] += A[2371] * x[950];
  y[951] += A[2372] * x[952];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 952] += A[1 * i0 + 2373] * x[1 * i0 + 951];
  }
  y[953] += A[2376] * x[952];
  y[953] += A[2377] * x[954];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 954] += A[1 * i0 + 2378] * x[1 * i0 + 953];
  }
  y[955] += A[2381] * x[954];
  y[955] += A[2382] * x[956];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 956] += A[1 * i0 + 2383] * x[1 * i0 + 955];
  }
  y[957] += A[2386] * x[956];
  y[957] += A[2387] * x[958];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 958] += A[1 * i0 + 2388] * x[1 * i0 + 957];
  }
  y[959] += A[2391] * x[958];
  y[959] += A[2392] * x[960];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 960] += A[1 * i0 + 2393] * x[1 * i0 + 959];
  }
  y[961] += A[2396] * x[960];
  y[961] += A[2397] * x[962];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 962] += A[1 * i0 + 2398] * x[1 * i0 + 961];
  }
  y[963] += A[2401] * x[962];
  y[963] += A[2402] * x[964];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 964] += A[1 * i0 + 2403] * x[1 * i0 + 963];
  }
  y[965] += A[2406] * x[964];
  y[965] += A[2407] * x[966];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 966] += A[1 * i0 + 2408] * x[1 * i0 + 965];
  }
  y[967] += A[2411] * x[966];
  y[967] += A[2412] * x[968];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 968] += A[1 * i0 + 2413] * x[1 * i0 + 967];
  }
  y[969] += A[2416] * x[968];
  y[969] += A[2417] * x[970];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 970] += A[1 * i0 + 2418] * x[1 * i0 + 969];
  }
  y[971] += A[2421] * x[970];
  y[971] += A[2422] * x[972];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 972] += A[1 * i0 + 2423] * x[1 * i0 + 971];
  }
  y[972] += A[2425] * x[1452];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 973] += A[1 * i0 + 2426] * x[1 * i0 + 973];
  }
  y[973] += A[2428] * x[1452];
  y[974] += A[2429] * x[973];
  y[974] += A[2430] * x[975];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 975] += A[1 * i0 + 2431] * x[1 * i0 + 974];
  }
  y[976] += A[2434] * x[975];
  y[976] += A[2435] * x[977];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 977] += A[1 * i0 + 2436] * x[1 * i0 + 976];
  }
  y[978] += A[2439] * x[977];
  y[978] += A[2440] * x[979];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 979] += A[1 * i0 + 2441] * x[1 * i0 + 978];
  }
  y[980] += A[2444] * x[979];
  y[980] += A[2445] * x[981];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 981] += A[1 * i0 + 2446] * x[1 * i0 + 980];
  }
  y[982] += A[2449] * x[981];
  y[982] += A[2450] * x[983];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 983] += A[1 * i0 + 2451] * x[1 * i0 + 982];
  }
  y[984] += A[2454] * x[983];
  y[984] += A[2455] * x[985];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 985] += A[1 * i0 + 2456] * x[1 * i0 + 984];
  }
  y[986] += A[2459] * x[985];
  y[986] += A[2460] * x[987];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 987] += A[1 * i0 + 2461] * x[1 * i0 + 986];
  }
  y[988] += A[2464] * x[987];
  y[988] += A[2465] * x[989];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 989] += A[1 * i0 + 2466] * x[1 * i0 + 988];
  }
  y[990] += A[2469] * x[989];
  y[990] += A[2470] * x[991];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 991] += A[1 * i0 + 2471] * x[1 * i0 + 990];
  }
  y[992] += A[2474] * x[991];
  y[992] += A[2475] * x[993];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 993] += A[1 * i0 + 2476] * x[1 * i0 + 992];
  }
  y[994] += A[2479] * x[993];
  y[994] += A[2480] * x[995];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 995] += A[1 * i0 + 2481] * x[1 * i0 + 994];
  }
  y[996] += A[2484] * x[995];
  y[996] += A[2485] * x[997];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 997] += A[1 * i0 + 2486] * x[1 * i0 + 996];
  }
  y[998] += A[2489] * x[997];
  y[998] += A[2490] * x[999];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 999] += A[1 * i0 + 2491] * x[1 * i0 + 998];
  }
  y[1000] += A[2494] * x[999];
  y[1000] += A[2495] * x[1001];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1001] += A[1 * i0 + 2496] * x[1 * i0 + 1000];
  }
  y[1002] += A[2499] * x[1001];
  y[1002] += A[2500] * x[1003];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1003] += A[1 * i0 + 2501] * x[1 * i0 + 1002];
  }
  y[1004] += A[2504] * x[1003];
  y[1004] += A[2505] * x[1005];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1005] += A[1 * i0 + 2506] * x[1 * i0 + 1004];
  }
  y[1006] += A[2509] * x[1005];
  y[1006] += A[2510] * x[1007];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1007] += A[1 * i0 + 2511] * x[1 * i0 + 1006];
  }
  y[1008] += A[2514] * x[1007];
  y[1008] += A[2515] * x[1009];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1009] += A[1 * i0 + 2516] * x[1 * i0 + 1008];
  }
  y[1010] += A[2519] * x[1009];
  y[1010] += A[2520] * x[1011];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1011] += A[1 * i0 + 2521] * x[1 * i0 + 1010];
  }
  y[1012] += A[2524] * x[1011];
  y[1012] += A[2525] * x[1013];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1013] += A[1 * i0 + 2526] * x[1 * i0 + 1012];
  }
  y[1014] += A[2529] * x[1013];
  y[1014] += A[2530] * x[1015];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1015] += A[1 * i0 + 2531] * x[1 * i0 + 1014];
  }
  y[1016] += A[2534] * x[1015];
  y[1016] += A[2535] * x[1017];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1017] += A[1 * i0 + 2536] * x[1 * i0 + 1016];
  }
  y[1018] += A[2539] * x[1017];
  y[1018] += A[2540] * x[1019];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1019] += A[1 * i0 + 2541] * x[1 * i0 + 1018];
  }
}

void kernel_spmv_fragment_2(double *restrict A, double *restrict x,
                            double *restrict y) {
  register int i0, i1, i2, i3, i4, i5, i6;

  y[1020] += A[2544] * x[1019];
  y[1020] += A[2545] * x[1021];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1021] += A[1 * i0 + 2546] * x[1 * i0 + 1020];
  }
  y[1022] += A[2549] * x[1021];
  y[1022] += A[2550] * x[1023];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1023] += A[1 * i0 + 2551] * x[1 * i0 + 1022];
  }
  y[1024] += A[2554] * x[1023];
  y[1024] += A[2555] * x[1025];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1025] += A[1 * i0 + 2556] * x[1 * i0 + 1024];
  }
  y[1026] += A[2559] * x[1025];
  y[1026] += A[2560] * x[1027];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1027] += A[1 * i0 + 2561] * x[1 * i0 + 1026];
  }
  y[1028] += A[2564] * x[1027];
  y[1028] += A[2565] * x[1029];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1029] += A[1 * i0 + 2566] * x[1 * i0 + 1028];
  }
  y[1030] += A[2569] * x[1029];
  y[1030] += A[2570] * x[1031];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1031] += A[1 * i0 + 2571] * x[1 * i0 + 1030];
  }
  y[1032] += A[2574] * x[1031];
  y[1032] += A[2575] * x[1033];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1033] += A[1 * i0 + 2576] * x[1 * i0 + 1032];
  }
  y[1034] += A[2579] * x[1033];
  y[1034] += A[2580] * x[1035];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1035] += A[1 * i0 + 2581] * x[1 * i0 + 1034];
  }
  y[1036] += A[2584] * x[1035];
  y[1036] += A[2585] * x[1037];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1037] += A[1 * i0 + 2586] * x[1 * i0 + 1036];
  }
  y[1038] += A[2589] * x[1037];
  y[1038] += A[2590] * x[1039];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1039] += A[1 * i0 + 2591] * x[1 * i0 + 1038];
  }
  y[1040] += A[2594] * x[1039];
  y[1040] += A[2595] * x[1041];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1041] += A[1 * i0 + 2596] * x[1 * i0 + 1040];
  }
  y[1042] += A[2599] * x[1041];
  y[1042] += A[2600] * x[1043];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1043] += A[1 * i0 + 2601] * x[1 * i0 + 1042];
  }
  y[1044] += A[2604] * x[1043];
  y[1044] += A[2605] * x[1045];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1045] += A[1 * i0 + 2606] * x[1 * i0 + 1044];
  }
  y[1046] += A[2609] * x[1045];
  y[1046] += A[2610] * x[1047];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1047] += A[1 * i0 + 2611] * x[1 * i0 + 1046];
  }
  y[1048] += A[2614] * x[1047];
  y[1048] += A[2615] * x[1049];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1049] += A[1 * i0 + 2616] * x[1 * i0 + 1048];
  }
  y[1050] += A[2619] * x[1049];
  y[1050] += A[2620] * x[1051];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1051] += A[1 * i0 + 2621] * x[1 * i0 + 1050];
  }
  y[1052] += A[2624] * x[1051];
  y[1052] += A[2625] * x[1053];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1053] += A[1 * i0 + 2626] * x[1 * i0 + 1052];
  }
  y[1054] += A[2629] * x[1053];
  y[1054] += A[2630] * x[1055];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1055] += A[1 * i0 + 2631] * x[1 * i0 + 1054];
  }
  y[1056] += A[2634] * x[1055];
  y[1056] += A[2635] * x[1057];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1057] += A[1 * i0 + 2636] * x[1 * i0 + 1056];
  }
  y[1058] += A[2639] * x[1057];
  y[1058] += A[2640] * x[1059];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1059] += A[1 * i0 + 2641] * x[1 * i0 + 1058];
  }
  y[1060] += A[2644] * x[1059];
  y[1060] += A[2645] * x[1061];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1061] += A[1 * i0 + 2646] * x[1 * i0 + 1060];
  }
  y[1062] += A[2649] * x[1061];
  y[1062] += A[2650] * x[1063];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1063] += A[1 * i0 + 2651] * x[1 * i0 + 1062];
  }
  y[1064] += A[2654] * x[1063];
  y[1064] += A[2655] * x[1065];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1065] += A[1 * i0 + 2656] * x[1 * i0 + 1064];
  }
  y[1066] += A[2659] * x[1065];
  y[1066] += A[2660] * x[1067];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1067] += A[1 * i0 + 2661] * x[1 * i0 + 1066];
  }
  y[1068] += A[2664] * x[1067];
  y[1068] += A[2665] * x[1069];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1069] += A[1 * i0 + 2666] * x[1 * i0 + 1068];
  }
  y[1070] += A[2669] * x[1069];
  y[1070] += A[2670] * x[1071];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1071] += A[1 * i0 + 2671] * x[1 * i0 + 1070];
  }
  y[1072] += A[2674] * x[1071];
  y[1072] += A[2675] * x[1073];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1073] += A[1 * i0 + 2676] * x[1 * i0 + 1072];
  }
  y[1074] += A[2679] * x[1073];
  y[1074] += A[2680] * x[1075];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1075] += A[1 * i0 + 2681] * x[1 * i0 + 1074];
  }
  y[1076] += A[2684] * x[1075];
  y[1076] += A[2685] * x[1077];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1077] += A[1 * i0 + 2686] * x[1 * i0 + 1076];
  }
  y[1078] += A[2689] * x[1077];
  y[1078] += A[2690] * x[1079];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1079] += A[1 * i0 + 2691] * x[1 * i0 + 1078];
  }
  y[1080] += A[2694] * x[1079];
  y[1080] += A[2695] * x[1081];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1081] += A[1 * i0 + 2696] * x[1 * i0 + 1080];
  }
  y[1082] += A[2699] * x[1081];
  y[1082] += A[2700] * x[1083];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1083] += A[1 * i0 + 2701] * x[1 * i0 + 1082];
  }
  y[1084] += A[2704] * x[1083];
  y[1084] += A[2705] * x[1085];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1085] += A[1 * i0 + 2706] * x[1 * i0 + 1084];
  }
  y[1086] += A[2709] * x[1085];
  y[1086] += A[2710] * x[1087];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1087] += A[1 * i0 + 2711] * x[1 * i0 + 1086];
  }
  y[1088] += A[2714] * x[1087];
  y[1088] += A[2715] * x[1089];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1089] += A[1 * i0 + 2716] * x[1 * i0 + 1088];
  }
  y[1090] += A[2719] * x[1089];
  y[1090] += A[2720] * x[1091];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1091] += A[1 * i0 + 2721] * x[1 * i0 + 1090];
  }
  y[1092] += A[2724] * x[1091];
  y[1092] += A[2725] * x[1093];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1093] += A[1 * i0 + 2726] * x[1 * i0 + 1092];
  }
  y[1094] += A[2729] * x[1093];
  y[1094] += A[2730] * x[1095];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1095] += A[1 * i0 + 2731] * x[1 * i0 + 1094];
  }
  y[1096] += A[2734] * x[1095];
  y[1096] += A[2735] * x[1097];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1097] += A[1 * i0 + 2736] * x[1 * i0 + 1096];
  }
  y[1098] += A[2739] * x[1097];
  y[1098] += A[2740] * x[1099];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1099] += A[1 * i0 + 2741] * x[1 * i0 + 1098];
  }
  y[1100] += A[2744] * x[1099];
  y[1100] += A[2745] * x[1101];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1101] += A[1 * i0 + 2746] * x[1 * i0 + 1100];
  }
  y[1102] += A[2749] * x[1101];
  y[1102] += A[2750] * x[1103];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1103] += A[1 * i0 + 2751] * x[1 * i0 + 1102];
  }
  y[1104] += A[2754] * x[1103];
  y[1104] += A[2755] * x[1105];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1105] += A[1 * i0 + 2756] * x[1 * i0 + 1104];
  }
  y[1106] += A[2759] * x[1105];
  y[1106] += A[2760] * x[1107];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1107] += A[1 * i0 + 2761] * x[1 * i0 + 1106];
  }
  y[1108] += A[2764] * x[1107];
  y[1108] += A[2765] * x[1109];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1109] += A[1 * i0 + 2766] * x[1 * i0 + 1108];
  }
  y[1110] += A[2769] * x[1109];
  y[1110] += A[2770] * x[1111];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1111] += A[1 * i0 + 2771] * x[1 * i0 + 1110];
  }
  y[1112] += A[2774] * x[1111];
  y[1112] += A[2775] * x[1113];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1113] += A[1 * i0 + 2776] * x[1 * i0 + 1112];
  }
  y[1114] += A[2779] * x[1113];
  y[1114] += A[2780] * x[1115];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1115] += A[1 * i0 + 2781] * x[1 * i0 + 1114];
  }
  y[1116] += A[2784] * x[1115];
  y[1116] += A[2785] * x[1117];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1117] += A[1 * i0 + 2786] * x[1 * i0 + 1116];
  }
  y[1118] += A[2789] * x[1117];
  y[1118] += A[2790] * x[1119];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1119] += A[1 * i0 + 2791] * x[1 * i0 + 1118];
  }
  y[1120] += A[2794] * x[1119];
  y[1120] += A[2795] * x[1121];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1121] += A[1 * i0 + 2796] * x[1 * i0 + 1120];
  }
  y[1122] += A[2799] * x[1121];
  y[1122] += A[2800] * x[1123];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1123] += A[1 * i0 + 2801] * x[1 * i0 + 1122];
  }
  y[1124] += A[2804] * x[1123];
  y[1124] += A[2805] * x[1125];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1125] += A[1 * i0 + 2806] * x[1 * i0 + 1124];
  }
  y[1126] += A[2809] * x[1125];
  y[1126] += A[2810] * x[1127];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1127] += A[1 * i0 + 2811] * x[1 * i0 + 1126];
  }
  y[1128] += A[2814] * x[1127];
  y[1128] += A[2815] * x[1129];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1129] += A[1 * i0 + 2816] * x[1 * i0 + 1128];
  }
  y[1130] += A[2819] * x[1129];
  y[1130] += A[2820] * x[1131];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1131] += A[1 * i0 + 2821] * x[1 * i0 + 1130];
  }
  y[1132] += A[2824] * x[495];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 1132] += A[1 * i0 + 2825] * x[1 * i0 + 1131];
  }
  y[1132] += A[2827] * x[4015];
  y[1132] += A[2828] * x[4021];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 1133] += A[1 * i0 + 2829] * x[1 * i0 + 1133];
  }
  y[1133] += A[2831] * x[1452];
  y[1134] += A[2832] * x[1133];
  y[1134] += A[2833] * x[1135];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1135] += A[1 * i0 + 2834] * x[1 * i0 + 1134];
  }
  y[1136] += A[2837] * x[1135];
  y[1136] += A[2838] * x[1137];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1137] += A[1 * i0 + 2839] * x[1 * i0 + 1136];
  }
  y[1138] += A[2842] * x[1137];
  y[1138] += A[2843] * x[1139];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1139] += A[1 * i0 + 2844] * x[1 * i0 + 1138];
  }
  y[1140] += A[2847] * x[1139];
  y[1140] += A[2848] * x[1141];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1141] += A[1 * i0 + 2849] * x[1 * i0 + 1140];
  }
  y[1142] += A[2852] * x[1141];
  y[1142] += A[2853] * x[1143];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1143] += A[1 * i0 + 2854] * x[1 * i0 + 1142];
  }
  y[1144] += A[2857] * x[1143];
  y[1144] += A[2858] * x[1145];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1145] += A[1 * i0 + 2859] * x[1 * i0 + 1144];
  }
  y[1146] += A[2862] * x[1145];
  y[1146] += A[2863] * x[1147];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1147] += A[1 * i0 + 2864] * x[1 * i0 + 1146];
  }
  y[1148] += A[2867] * x[1147];
  y[1148] += A[2868] * x[1149];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1149] += A[1 * i0 + 2869] * x[1 * i0 + 1148];
  }
  y[1150] += A[2872] * x[1149];
  y[1150] += A[2873] * x[1151];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1151] += A[1 * i0 + 2874] * x[1 * i0 + 1150];
  }
  y[1152] += A[2877] * x[1151];
  y[1152] += A[2878] * x[1153];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1153] += A[1 * i0 + 2879] * x[1 * i0 + 1152];
  }
  y[1154] += A[2882] * x[1153];
  y[1154] += A[2883] * x[1155];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1155] += A[1 * i0 + 2884] * x[1 * i0 + 1154];
  }
  y[1156] += A[2887] * x[1155];
  y[1156] += A[2888] * x[1157];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1157] += A[1 * i0 + 2889] * x[1 * i0 + 1156];
  }
  y[1158] += A[2892] * x[1157];
  y[1158] += A[2893] * x[1159];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1159] += A[1 * i0 + 2894] * x[1 * i0 + 1158];
  }
  y[1160] += A[2897] * x[1159];
  y[1160] += A[2898] * x[1161];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1161] += A[1 * i0 + 2899] * x[1 * i0 + 1160];
  }
  y[1162] += A[2902] * x[1161];
  y[1162] += A[2903] * x[1163];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1163] += A[1 * i0 + 2904] * x[1 * i0 + 1162];
  }
  y[1164] += A[2907] * x[1163];
  y[1164] += A[2908] * x[1165];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1165] += A[1 * i0 + 2909] * x[1 * i0 + 1164];
  }
  y[1166] += A[2912] * x[1165];
  y[1166] += A[2913] * x[1167];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1167] += A[1 * i0 + 2914] * x[1 * i0 + 1166];
  }
  y[1168] += A[2917] * x[1167];
  y[1168] += A[2918] * x[1169];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1169] += A[1 * i0 + 2919] * x[1 * i0 + 1168];
  }
  y[1170] += A[2922] * x[1169];
  y[1170] += A[2923] * x[1171];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1171] += A[1 * i0 + 2924] * x[1 * i0 + 1170];
  }
  y[1172] += A[2927] * x[1171];
  y[1172] += A[2928] * x[1173];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1173] += A[1 * i0 + 2929] * x[1 * i0 + 1172];
  }
  y[1174] += A[2932] * x[1173];
  y[1174] += A[2933] * x[1175];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1175] += A[1 * i0 + 2934] * x[1 * i0 + 1174];
  }
  y[1176] += A[2937] * x[1175];
  y[1176] += A[2938] * x[1177];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1177] += A[1 * i0 + 2939] * x[1 * i0 + 1176];
  }
  y[1178] += A[2942] * x[1177];
  y[1178] += A[2943] * x[1179];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1179] += A[1 * i0 + 2944] * x[1 * i0 + 1178];
  }
  y[1180] += A[2947] * x[1179];
  y[1180] += A[2948] * x[1181];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1181] += A[1 * i0 + 2949] * x[1 * i0 + 1180];
  }
  y[1182] += A[2952] * x[1181];
  y[1182] += A[2953] * x[1183];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1183] += A[1 * i0 + 2954] * x[1 * i0 + 1182];
  }
  y[1184] += A[2957] * x[1183];
  y[1184] += A[2958] * x[1185];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1185] += A[1 * i0 + 2959] * x[1 * i0 + 1184];
  }
  y[1186] += A[2962] * x[1185];
  y[1186] += A[2963] * x[1187];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1187] += A[1 * i0 + 2964] * x[1 * i0 + 1186];
  }
  y[1188] += A[2967] * x[1187];
  y[1188] += A[2968] * x[1189];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1189] += A[1 * i0 + 2969] * x[1 * i0 + 1188];
  }
  y[1190] += A[2972] * x[1189];
  y[1190] += A[2973] * x[1191];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1191] += A[1 * i0 + 2974] * x[1 * i0 + 1190];
  }
  y[1192] += A[2977] * x[1191];
  y[1192] += A[2978] * x[1193];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1193] += A[1 * i0 + 2979] * x[1 * i0 + 1192];
  }
  y[1194] += A[2982] * x[1193];
  y[1194] += A[2983] * x[1195];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1195] += A[1 * i0 + 2984] * x[1 * i0 + 1194];
  }
  y[1196] += A[2987] * x[1195];
  y[1196] += A[2988] * x[1197];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1197] += A[1 * i0 + 2989] * x[1 * i0 + 1196];
  }
  y[1198] += A[2992] * x[1197];
  y[1198] += A[2993] * x[1199];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1199] += A[1 * i0 + 2994] * x[1 * i0 + 1198];
  }
  y[1200] += A[2997] * x[1199];
  y[1200] += A[2998] * x[1201];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1201] += A[1 * i0 + 2999] * x[1 * i0 + 1200];
  }
  y[1202] += A[3002] * x[1201];
  y[1202] += A[3003] * x[1203];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1203] += A[1 * i0 + 3004] * x[1 * i0 + 1202];
  }
  y[1204] += A[3007] * x[1203];
  y[1204] += A[3008] * x[1205];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1205] += A[1 * i0 + 3009] * x[1 * i0 + 1204];
  }
  y[1206] += A[3012] * x[1205];
  y[1206] += A[3013] * x[1207];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1207] += A[1 * i0 + 3014] * x[1 * i0 + 1206];
  }
  y[1208] += A[3017] * x[1207];
  y[1208] += A[3018] * x[1209];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1209] += A[1 * i0 + 3019] * x[1 * i0 + 1208];
  }
  y[1210] += A[3022] * x[1209];
  y[1210] += A[3023] * x[1211];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1211] += A[1 * i0 + 3024] * x[1 * i0 + 1210];
  }
  y[1212] += A[3027] * x[1211];
  y[1212] += A[3028] * x[1213];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1213] += A[1 * i0 + 3029] * x[1 * i0 + 1212];
  }
  y[1214] += A[3032] * x[1213];
  y[1214] += A[3033] * x[1215];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1215] += A[1 * i0 + 3034] * x[1 * i0 + 1214];
  }
  y[1216] += A[3037] * x[1215];
  y[1216] += A[3038] * x[1217];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1217] += A[1 * i0 + 3039] * x[1 * i0 + 1216];
  }
  y[1218] += A[3042] * x[1217];
  y[1218] += A[3043] * x[1219];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1219] += A[1 * i0 + 3044] * x[1 * i0 + 1218];
  }
  y[1220] += A[3047] * x[1219];
  y[1220] += A[3048] * x[1221];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1221] += A[1 * i0 + 3049] * x[1 * i0 + 1220];
  }
  y[1222] += A[3052] * x[1221];
  y[1222] += A[3053] * x[1223];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1223] += A[1 * i0 + 3054] * x[1 * i0 + 1222];
  }
  y[1224] += A[3057] * x[1223];
  y[1224] += A[3058] * x[1225];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1225] += A[1 * i0 + 3059] * x[1 * i0 + 1224];
  }
  y[1226] += A[3062] * x[1225];
  y[1226] += A[3063] * x[1227];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1227] += A[1 * i0 + 3064] * x[1 * i0 + 1226];
  }
  y[1228] += A[3067] * x[1227];
  y[1228] += A[3068] * x[1229];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1229] += A[1 * i0 + 3069] * x[1 * i0 + 1228];
  }
  y[1230] += A[3072] * x[1229];
  y[1230] += A[3073] * x[1231];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1231] += A[1 * i0 + 3074] * x[1 * i0 + 1230];
  }
  y[1232] += A[3077] * x[1231];
  y[1232] += A[3078] * x[1233];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1233] += A[1 * i0 + 3079] * x[1 * i0 + 1232];
  }
  y[1234] += A[3082] * x[1233];
  y[1234] += A[3083] * x[1235];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1235] += A[1 * i0 + 3084] * x[1 * i0 + 1234];
  }
  y[1236] += A[3087] * x[1235];
  y[1236] += A[3088] * x[1237];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1237] += A[1 * i0 + 3089] * x[1 * i0 + 1236];
  }
  y[1238] += A[3092] * x[1237];
  y[1238] += A[3093] * x[1239];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1239] += A[1 * i0 + 3094] * x[1 * i0 + 1238];
  }
  y[1240] += A[3097] * x[1239];
  y[1240] += A[3098] * x[1241];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1241] += A[1 * i0 + 3099] * x[1 * i0 + 1240];
  }
  y[1242] += A[3102] * x[1241];
  y[1242] += A[3103] * x[1243];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1243] += A[1 * i0 + 3104] * x[1 * i0 + 1242];
  }
  y[1244] += A[3107] * x[1243];
  y[1244] += A[3108] * x[1245];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1245] += A[1 * i0 + 3109] * x[1 * i0 + 1244];
  }
  y[1246] += A[3112] * x[1245];
  y[1246] += A[3113] * x[1247];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1247] += A[1 * i0 + 3114] * x[1 * i0 + 1246];
  }
  y[1248] += A[3117] * x[1247];
  y[1248] += A[3118] * x[1249];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1249] += A[1 * i0 + 3119] * x[1 * i0 + 1248];
  }
  y[1250] += A[3122] * x[1249];
  y[1250] += A[3123] * x[1251];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1251] += A[1 * i0 + 3124] * x[1 * i0 + 1250];
  }
  y[1252] += A[3127] * x[1251];
  y[1252] += A[3128] * x[1253];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1253] += A[1 * i0 + 3129] * x[1 * i0 + 1252];
  }
  y[1254] += A[3132] * x[1253];
  y[1254] += A[3133] * x[1255];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1255] += A[1 * i0 + 3134] * x[1 * i0 + 1254];
  }
  y[1256] += A[3137] * x[1255];
  y[1256] += A[3138] * x[1257];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1257] += A[1 * i0 + 3139] * x[1 * i0 + 1256];
  }
  y[1258] += A[3142] * x[1257];
  y[1258] += A[3143] * x[1259];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1259] += A[1 * i0 + 3144] * x[1 * i0 + 1258];
  }
  y[1260] += A[3147] * x[1259];
  y[1260] += A[3148] * x[1261];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1261] += A[1 * i0 + 3149] * x[1 * i0 + 1260];
  }
  y[1262] += A[3152] * x[1261];
  y[1262] += A[3153] * x[1263];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1263] += A[1 * i0 + 3154] * x[1 * i0 + 1262];
  }
  y[1264] += A[3157] * x[1263];
  y[1264] += A[3158] * x[1265];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1265] += A[1 * i0 + 3159] * x[1 * i0 + 1264];
  }
  y[1266] += A[3162] * x[1265];
  y[1266] += A[3163] * x[1267];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1267] += A[1 * i0 + 3164] * x[1 * i0 + 1266];
  }
  y[1268] += A[3167] * x[1267];
  y[1268] += A[3168] * x[1269];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1269] += A[1 * i0 + 3169] * x[1 * i0 + 1268];
  }
  y[1270] += A[3172] * x[1269];
  y[1270] += A[3173] * x[1271];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1271] += A[1 * i0 + 3174] * x[1 * i0 + 1270];
  }
  y[1272] += A[3177] * x[1271];
  y[1272] += A[3178] * x[1273];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1273] += A[1 * i0 + 3179] * x[1 * i0 + 1272];
  }
  y[1274] += A[3182] * x[1273];
  y[1274] += A[3183] * x[1275];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1275] += A[1 * i0 + 3184] * x[1 * i0 + 1274];
  }
  y[1276] += A[3187] * x[1275];
  y[1276] += A[3188] * x[1277];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1277] += A[1 * i0 + 3189] * x[1 * i0 + 1276];
  }
  y[1278] += A[3192] * x[1277];
  y[1278] += A[3193] * x[1279];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1279] += A[1 * i0 + 3194] * x[1 * i0 + 1278];
  }
  y[1280] += A[3197] * x[1279];
  y[1280] += A[3198] * x[1281];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1281] += A[1 * i0 + 3199] * x[1 * i0 + 1280];
  }
  y[1282] += A[3202] * x[1281];
  y[1282] += A[3203] * x[1283];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1283] += A[1 * i0 + 3204] * x[1 * i0 + 1282];
  }
  y[1284] += A[3207] * x[1283];
  y[1284] += A[3208] * x[1285];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1285] += A[1 * i0 + 3209] * x[1 * i0 + 1284];
  }
  y[1286] += A[3212] * x[1285];
  y[1286] += A[3213] * x[1287];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1287] += A[1 * i0 + 3214] * x[1 * i0 + 1286];
  }
  y[1288] += A[3217] * x[1287];
  y[1288] += A[3218] * x[1289];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1289] += A[1 * i0 + 3219] * x[1 * i0 + 1288];
  }
  y[1290] += A[3222] * x[1289];
  y[1290] += A[3223] * x[1291];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 1291] += A[1 * i0 + 3224] * x[1 * i0 + 1290];
  }
  y[1291] += A[3226] * x[1772];
  y[1292] += A[3227] * x[1293];
  y[1292] += A[3228] * x[4018];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1293] += A[1 * i0 + 3229] * x[1 * i0 + 1292];
  }
  y[1294] += A[3232] * x[1293];
  y[1294] += A[3233] * x[1295];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1295] += A[1 * i0 + 3234] * x[1 * i0 + 1294];
  }
  y[1296] += A[3237] * x[1295];
  y[1296] += A[3238] * x[1297];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1297] += A[1 * i0 + 3239] * x[1 * i0 + 1296];
  }
  y[1298] += A[3242] * x[1297];
  y[1298] += A[3243] * x[1299];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1299] += A[1 * i0 + 3244] * x[1 * i0 + 1298];
  }
  y[1300] += A[3247] * x[1299];
  y[1300] += A[3248] * x[1301];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1301] += A[1 * i0 + 3249] * x[1 * i0 + 1300];
  }
  y[1302] += A[3252] * x[1301];
  y[1302] += A[3253] * x[1303];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1303] += A[1 * i0 + 3254] * x[1 * i0 + 1302];
  }
  y[1304] += A[3257] * x[1303];
  y[1304] += A[3258] * x[1305];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1305] += A[1 * i0 + 3259] * x[1 * i0 + 1304];
  }
  y[1306] += A[3262] * x[1305];
  y[1306] += A[3263] * x[1307];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1307] += A[1 * i0 + 3264] * x[1 * i0 + 1306];
  }
  y[1308] += A[3267] * x[1307];
  y[1308] += A[3268] * x[1309];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1309] += A[1 * i0 + 3269] * x[1 * i0 + 1308];
  }
  y[1310] += A[3272] * x[1309];
  y[1310] += A[3273] * x[1311];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1311] += A[1 * i0 + 3274] * x[1 * i0 + 1310];
  }
  y[1312] += A[3277] * x[1311];
  y[1312] += A[3278] * x[1313];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1313] += A[1 * i0 + 3279] * x[1 * i0 + 1312];
  }
  y[1314] += A[3282] * x[1313];
  y[1314] += A[3283] * x[1315];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1315] += A[1 * i0 + 3284] * x[1 * i0 + 1314];
  }
  y[1316] += A[3287] * x[1315];
  y[1316] += A[3288] * x[1317];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1317] += A[1 * i0 + 3289] * x[1 * i0 + 1316];
  }
  y[1318] += A[3292] * x[1317];
  y[1318] += A[3293] * x[1319];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1319] += A[1 * i0 + 3294] * x[1 * i0 + 1318];
  }
  y[1320] += A[3297] * x[1319];
  y[1320] += A[3298] * x[1321];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1321] += A[1 * i0 + 3299] * x[1 * i0 + 1320];
  }
  y[1322] += A[3302] * x[1321];
  y[1322] += A[3303] * x[1323];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1323] += A[1 * i0 + 3304] * x[1 * i0 + 1322];
  }
  y[1324] += A[3307] * x[1323];
  y[1324] += A[3308] * x[1325];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1325] += A[1 * i0 + 3309] * x[1 * i0 + 1324];
  }
  y[1326] += A[3312] * x[1325];
  y[1326] += A[3313] * x[1327];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1327] += A[1 * i0 + 3314] * x[1 * i0 + 1326];
  }
  y[1328] += A[3317] * x[1327];
  y[1328] += A[3318] * x[1329];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1329] += A[1 * i0 + 3319] * x[1 * i0 + 1328];
  }
  y[1330] += A[3322] * x[1329];
  y[1330] += A[3323] * x[1331];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1331] += A[1 * i0 + 3324] * x[1 * i0 + 1330];
  }
  y[1332] += A[3327] * x[1331];
  y[1332] += A[3328] * x[1333];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1333] += A[1 * i0 + 3329] * x[1 * i0 + 1332];
  }
  y[1334] += A[3332] * x[1333];
  y[1334] += A[3333] * x[1335];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1335] += A[1 * i0 + 3334] * x[1 * i0 + 1334];
  }
  y[1336] += A[3337] * x[1335];
  y[1336] += A[3338] * x[1337];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1337] += A[1 * i0 + 3339] * x[1 * i0 + 1336];
  }
  y[1338] += A[3342] * x[1337];
  y[1338] += A[3343] * x[1339];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1339] += A[1 * i0 + 3344] * x[1 * i0 + 1338];
  }
  y[1340] += A[3347] * x[1339];
  y[1340] += A[3348] * x[1341];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1341] += A[1 * i0 + 3349] * x[1 * i0 + 1340];
  }
  y[1342] += A[3352] * x[1341];
  y[1342] += A[3353] * x[1343];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1343] += A[1 * i0 + 3354] * x[1 * i0 + 1342];
  }
  y[1344] += A[3357] * x[1343];
  y[1344] += A[3358] * x[1345];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1345] += A[1 * i0 + 3359] * x[1 * i0 + 1344];
  }
  y[1346] += A[3362] * x[1345];
  y[1346] += A[3363] * x[1347];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1347] += A[1 * i0 + 3364] * x[1 * i0 + 1346];
  }
  y[1348] += A[3367] * x[1347];
  y[1348] += A[3368] * x[1349];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1349] += A[1 * i0 + 3369] * x[1 * i0 + 1348];
  }
  y[1350] += A[3372] * x[1349];
  y[1350] += A[3373] * x[1351];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1351] += A[1 * i0 + 3374] * x[1 * i0 + 1350];
  }
  y[1352] += A[3377] * x[1351];
  y[1352] += A[3378] * x[1353];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1353] += A[1 * i0 + 3379] * x[1 * i0 + 1352];
  }
  y[1354] += A[3382] * x[1353];
  y[1354] += A[3383] * x[1355];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1355] += A[1 * i0 + 3384] * x[1 * i0 + 1354];
  }
  y[1356] += A[3387] * x[1355];
  y[1356] += A[3388] * x[1357];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1357] += A[1 * i0 + 3389] * x[1 * i0 + 1356];
  }
  y[1358] += A[3392] * x[1357];
  y[1358] += A[3393] * x[1359];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1359] += A[1 * i0 + 3394] * x[1 * i0 + 1358];
  }
  y[1360] += A[3397] * x[1359];
  y[1360] += A[3398] * x[1361];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1361] += A[1 * i0 + 3399] * x[1 * i0 + 1360];
  }
  y[1362] += A[3402] * x[1361];
  y[1362] += A[3403] * x[1363];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1363] += A[1 * i0 + 3404] * x[1 * i0 + 1362];
  }
  y[1364] += A[3407] * x[1363];
  y[1364] += A[3408] * x[1365];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1365] += A[1 * i0 + 3409] * x[1 * i0 + 1364];
  }
  y[1366] += A[3412] * x[1365];
  y[1366] += A[3413] * x[1367];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1367] += A[1 * i0 + 3414] * x[1 * i0 + 1366];
  }
  y[1368] += A[3417] * x[1367];
  y[1368] += A[3418] * x[1369];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1369] += A[1 * i0 + 3419] * x[1 * i0 + 1368];
  }
  y[1370] += A[3422] * x[1369];
  y[1370] += A[3423] * x[1371];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1371] += A[1 * i0 + 3424] * x[1 * i0 + 1370];
  }
  y[1372] += A[3427] * x[1371];
  y[1372] += A[3428] * x[1373];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1373] += A[1 * i0 + 3429] * x[1 * i0 + 1372];
  }
  y[1374] += A[3432] * x[1373];
  y[1374] += A[3433] * x[1375];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1375] += A[1 * i0 + 3434] * x[1 * i0 + 1374];
  }
  y[1376] += A[3437] * x[1375];
  y[1376] += A[3438] * x[1377];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1377] += A[1 * i0 + 3439] * x[1 * i0 + 1376];
  }
  y[1378] += A[3442] * x[1377];
  y[1378] += A[3443] * x[1379];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1379] += A[1 * i0 + 3444] * x[1 * i0 + 1378];
  }
  y[1380] += A[3447] * x[1379];
  y[1380] += A[3448] * x[1381];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1381] += A[1 * i0 + 3449] * x[1 * i0 + 1380];
  }
  y[1382] += A[3452] * x[1381];
  y[1382] += A[3453] * x[1383];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1383] += A[1 * i0 + 3454] * x[1 * i0 + 1382];
  }
  y[1384] += A[3457] * x[1383];
  y[1384] += A[3458] * x[1385];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1385] += A[1 * i0 + 3459] * x[1 * i0 + 1384];
  }
  y[1386] += A[3462] * x[1385];
  y[1386] += A[3463] * x[1387];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1387] += A[1 * i0 + 3464] * x[1 * i0 + 1386];
  }
  y[1388] += A[3467] * x[1387];
  y[1388] += A[3468] * x[1389];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1389] += A[1 * i0 + 3469] * x[1 * i0 + 1388];
  }
  y[1390] += A[3472] * x[1389];
  y[1390] += A[3473] * x[1391];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1391] += A[1 * i0 + 3474] * x[1 * i0 + 1390];
  }
  y[1392] += A[3477] * x[1391];
  y[1392] += A[3478] * x[1393];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1393] += A[1 * i0 + 3479] * x[1 * i0 + 1392];
  }
  y[1394] += A[3482] * x[1393];
  y[1394] += A[3483] * x[1395];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1395] += A[1 * i0 + 3484] * x[1 * i0 + 1394];
  }
  y[1396] += A[3487] * x[1395];
  y[1396] += A[3488] * x[1397];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1397] += A[1 * i0 + 3489] * x[1 * i0 + 1396];
  }
  y[1398] += A[3492] * x[1397];
  y[1398] += A[3493] * x[1399];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1399] += A[1 * i0 + 3494] * x[1 * i0 + 1398];
  }
  y[1400] += A[3497] * x[1399];
  y[1400] += A[3498] * x[1401];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1401] += A[1 * i0 + 3499] * x[1 * i0 + 1400];
  }
  y[1402] += A[3502] * x[1401];
  y[1402] += A[3503] * x[1403];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1403] += A[1 * i0 + 3504] * x[1 * i0 + 1402];
  }
  y[1404] += A[3507] * x[1403];
  y[1404] += A[3508] * x[1405];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1405] += A[1 * i0 + 3509] * x[1 * i0 + 1404];
  }
  y[1406] += A[3512] * x[1405];
  y[1406] += A[3513] * x[1407];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1407] += A[1 * i0 + 3514] * x[1 * i0 + 1406];
  }
  y[1408] += A[3517] * x[1407];
  y[1408] += A[3518] * x[1409];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1409] += A[1 * i0 + 3519] * x[1 * i0 + 1408];
  }
  y[1410] += A[3522] * x[1409];
  y[1410] += A[3523] * x[1411];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1411] += A[1 * i0 + 3524] * x[1 * i0 + 1410];
  }
  y[1412] += A[3527] * x[1411];
  y[1412] += A[3528] * x[1413];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1413] += A[1 * i0 + 3529] * x[1 * i0 + 1412];
  }
  y[1414] += A[3532] * x[1413];
  y[1414] += A[3533] * x[1415];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1415] += A[1 * i0 + 3534] * x[1 * i0 + 1414];
  }
  y[1416] += A[3537] * x[1415];
  y[1416] += A[3538] * x[1417];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1417] += A[1 * i0 + 3539] * x[1 * i0 + 1416];
  }
  y[1418] += A[3542] * x[1417];
  y[1418] += A[3543] * x[1419];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1419] += A[1 * i0 + 3544] * x[1 * i0 + 1418];
  }
  y[1420] += A[3547] * x[1419];
  y[1420] += A[3548] * x[1421];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1421] += A[1 * i0 + 3549] * x[1 * i0 + 1420];
  }
  y[1422] += A[3552] * x[1421];
  y[1422] += A[3553] * x[1423];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1423] += A[1 * i0 + 3554] * x[1 * i0 + 1422];
  }
  y[1424] += A[3557] * x[1423];
  y[1424] += A[3558] * x[1425];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1425] += A[1 * i0 + 3559] * x[1 * i0 + 1424];
  }
  y[1426] += A[3562] * x[1425];
  y[1426] += A[3563] * x[1427];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1427] += A[1 * i0 + 3564] * x[1 * i0 + 1426];
  }
  y[1428] += A[3567] * x[1427];
  y[1428] += A[3568] * x[1429];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1429] += A[1 * i0 + 3569] * x[1 * i0 + 1428];
  }
  y[1430] += A[3572] * x[1429];
  y[1430] += A[3573] * x[1431];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1431] += A[1 * i0 + 3574] * x[1 * i0 + 1430];
  }
  y[1432] += A[3577] * x[1431];
  y[1432] += A[3578] * x[1433];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1433] += A[1 * i0 + 3579] * x[1 * i0 + 1432];
  }
  y[1434] += A[3582] * x[1433];
  y[1434] += A[3583] * x[1435];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1435] += A[1 * i0 + 3584] * x[1 * i0 + 1434];
  }
  y[1436] += A[3587] * x[1435];
  y[1436] += A[3588] * x[1437];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1437] += A[1 * i0 + 3589] * x[1 * i0 + 1436];
  }
  y[1438] += A[3592] * x[1437];
  y[1438] += A[3593] * x[1439];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1439] += A[1 * i0 + 3594] * x[1 * i0 + 1438];
  }
  y[1440] += A[3597] * x[1439];
  y[1440] += A[3598] * x[1441];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1441] += A[1 * i0 + 3599] * x[1 * i0 + 1440];
  }
  y[1442] += A[3602] * x[1441];
  y[1442] += A[3603] * x[1443];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1443] += A[1 * i0 + 3604] * x[1 * i0 + 1442];
  }
  y[1444] += A[3607] * x[1443];
  y[1444] += A[3608] * x[1445];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1445] += A[1 * i0 + 3609] * x[1 * i0 + 1444];
  }
  y[1446] += A[3612] * x[1445];
  y[1446] += A[3613] * x[1447];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1447] += A[1 * i0 + 3614] * x[1 * i0 + 1446];
  }
  y[1448] += A[3617] * x[1447];
  y[1448] += A[3618] * x[1449];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1449] += A[1 * i0 + 3619] * x[1 * i0 + 1448];
  }
  y[1450] += A[3622] * x[1449];
  y[1450] += A[3623] * x[1451];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1451] += A[1 * i0 + 3624] * x[1 * i0 + 1450];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 1452] += A[1 * i0 + 3627] * x[1 * i0 + 5];
  }
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 1452] += A[1 * i0 + 3629] * x[1 * i0 + 972];
  }
  y[1452] += A[3631] * x[1133];
  y[1452] += A[3632] * x[1451];
  y[1452] += A[3633] * x[4022];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 1453] += A[1 * i0 + 3634] * x[1 * i0 + 1453];
  }
  y[1453] += A[3636] * x[4012];
  y[1454] += A[3637] * x[1453];
  y[1454] += A[3638] * x[1455];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1455] += A[1 * i0 + 3639] * x[1 * i0 + 1454];
  }
  y[1456] += A[3642] * x[1455];
  y[1456] += A[3643] * x[1457];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1457] += A[1 * i0 + 3644] * x[1 * i0 + 1456];
  }
  y[1458] += A[3647] * x[1457];
  y[1458] += A[3648] * x[1459];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1459] += A[1 * i0 + 3649] * x[1 * i0 + 1458];
  }
  y[1460] += A[3652] * x[1459];
  y[1460] += A[3653] * x[1461];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1461] += A[1 * i0 + 3654] * x[1 * i0 + 1460];
  }
  y[1462] += A[3657] * x[1461];
  y[1462] += A[3658] * x[1463];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1463] += A[1 * i0 + 3659] * x[1 * i0 + 1462];
  }
  y[1464] += A[3662] * x[1463];
  y[1464] += A[3663] * x[1465];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1465] += A[1 * i0 + 3664] * x[1 * i0 + 1464];
  }
  y[1466] += A[3667] * x[1465];
  y[1466] += A[3668] * x[1467];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1467] += A[1 * i0 + 3669] * x[1 * i0 + 1466];
  }
  y[1468] += A[3672] * x[1467];
  y[1468] += A[3673] * x[1469];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1469] += A[1 * i0 + 3674] * x[1 * i0 + 1468];
  }
  y[1470] += A[3677] * x[1469];
  y[1470] += A[3678] * x[1471];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1471] += A[1 * i0 + 3679] * x[1 * i0 + 1470];
  }
  y[1472] += A[3682] * x[1471];
  y[1472] += A[3683] * x[1473];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1473] += A[1 * i0 + 3684] * x[1 * i0 + 1472];
  }
  y[1474] += A[3687] * x[1473];
  y[1474] += A[3688] * x[1475];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1475] += A[1 * i0 + 3689] * x[1 * i0 + 1474];
  }
  y[1476] += A[3692] * x[1475];
  y[1476] += A[3693] * x[1477];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1477] += A[1 * i0 + 3694] * x[1 * i0 + 1476];
  }
  y[1478] += A[3697] * x[1477];
  y[1478] += A[3698] * x[1479];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1479] += A[1 * i0 + 3699] * x[1 * i0 + 1478];
  }
  y[1480] += A[3702] * x[1479];
  y[1480] += A[3703] * x[1481];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1481] += A[1 * i0 + 3704] * x[1 * i0 + 1480];
  }
  y[1482] += A[3707] * x[1481];
  y[1482] += A[3708] * x[1483];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1483] += A[1 * i0 + 3709] * x[1 * i0 + 1482];
  }
  y[1484] += A[3712] * x[1483];
  y[1484] += A[3713] * x[1485];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1485] += A[1 * i0 + 3714] * x[1 * i0 + 1484];
  }
  y[1486] += A[3717] * x[1485];
  y[1486] += A[3718] * x[1487];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1487] += A[1 * i0 + 3719] * x[1 * i0 + 1486];
  }
  y[1488] += A[3722] * x[1487];
  y[1488] += A[3723] * x[1489];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1489] += A[1 * i0 + 3724] * x[1 * i0 + 1488];
  }
  y[1490] += A[3727] * x[1489];
  y[1490] += A[3728] * x[1491];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1491] += A[1 * i0 + 3729] * x[1 * i0 + 1490];
  }
  y[1492] += A[3732] * x[1491];
  y[1492] += A[3733] * x[1493];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1493] += A[1 * i0 + 3734] * x[1 * i0 + 1492];
  }
  y[1494] += A[3737] * x[1493];
  y[1494] += A[3738] * x[1495];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1495] += A[1 * i0 + 3739] * x[1 * i0 + 1494];
  }
  y[1496] += A[3742] * x[1495];
  y[1496] += A[3743] * x[1497];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1497] += A[1 * i0 + 3744] * x[1 * i0 + 1496];
  }
  y[1498] += A[3747] * x[1497];
  y[1498] += A[3748] * x[1499];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1499] += A[1 * i0 + 3749] * x[1 * i0 + 1498];
  }
  y[1500] += A[3752] * x[1499];
  y[1500] += A[3753] * x[1501];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1501] += A[1 * i0 + 3754] * x[1 * i0 + 1500];
  }
  y[1502] += A[3757] * x[1501];
  y[1502] += A[3758] * x[1503];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1503] += A[1 * i0 + 3759] * x[1 * i0 + 1502];
  }
  y[1504] += A[3762] * x[1503];
  y[1504] += A[3763] * x[1505];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1505] += A[1 * i0 + 3764] * x[1 * i0 + 1504];
  }
  y[1506] += A[3767] * x[1505];
  y[1506] += A[3768] * x[1507];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1507] += A[1 * i0 + 3769] * x[1 * i0 + 1506];
  }
  y[1508] += A[3772] * x[1507];
  y[1508] += A[3773] * x[1509];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1509] += A[1 * i0 + 3774] * x[1 * i0 + 1508];
  }
  y[1510] += A[3777] * x[1509];
  y[1510] += A[3778] * x[1511];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1511] += A[1 * i0 + 3779] * x[1 * i0 + 1510];
  }
  y[1512] += A[3782] * x[1511];
  y[1512] += A[3783] * x[1513];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1513] += A[1 * i0 + 3784] * x[1 * i0 + 1512];
  }
  y[1514] += A[3787] * x[1513];
  y[1514] += A[3788] * x[1515];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1515] += A[1 * i0 + 3789] * x[1 * i0 + 1514];
  }
  y[1516] += A[3792] * x[1515];
  y[1516] += A[3793] * x[1517];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1517] += A[1 * i0 + 3794] * x[1 * i0 + 1516];
  }
  y[1518] += A[3797] * x[1517];
  y[1518] += A[3798] * x[1519];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1519] += A[1 * i0 + 3799] * x[1 * i0 + 1518];
  }
}

void kernel_spmv_fragment_3(double *restrict A, double *restrict x,
                            double *restrict y) {
  register int i0, i1, i2, i3, i4, i5, i6;

  y[1520] += A[3802] * x[1519];
  y[1520] += A[3803] * x[1521];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1521] += A[1 * i0 + 3804] * x[1 * i0 + 1520];
  }
  y[1522] += A[3807] * x[1521];
  y[1522] += A[3808] * x[1523];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1523] += A[1 * i0 + 3809] * x[1 * i0 + 1522];
  }
  y[1524] += A[3812] * x[1523];
  y[1524] += A[3813] * x[1525];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1525] += A[1 * i0 + 3814] * x[1 * i0 + 1524];
  }
  y[1526] += A[3817] * x[1525];
  y[1526] += A[3818] * x[1527];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1527] += A[1 * i0 + 3819] * x[1 * i0 + 1526];
  }
  y[1528] += A[3822] * x[1527];
  y[1528] += A[3823] * x[1529];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1529] += A[1 * i0 + 3824] * x[1 * i0 + 1528];
  }
  y[1530] += A[3827] * x[1529];
  y[1530] += A[3828] * x[1531];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1531] += A[1 * i0 + 3829] * x[1 * i0 + 1530];
  }
  y[1532] += A[3832] * x[1531];
  y[1532] += A[3833] * x[1533];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1533] += A[1 * i0 + 3834] * x[1 * i0 + 1532];
  }
  y[1534] += A[3837] * x[1533];
  y[1534] += A[3838] * x[1535];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1535] += A[1 * i0 + 3839] * x[1 * i0 + 1534];
  }
  y[1536] += A[3842] * x[1535];
  y[1536] += A[3843] * x[1537];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1537] += A[1 * i0 + 3844] * x[1 * i0 + 1536];
  }
  y[1538] += A[3847] * x[1537];
  y[1538] += A[3848] * x[1539];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1539] += A[1 * i0 + 3849] * x[1 * i0 + 1538];
  }
  y[1540] += A[3852] * x[1539];
  y[1540] += A[3853] * x[1541];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1541] += A[1 * i0 + 3854] * x[1 * i0 + 1540];
  }
  y[1542] += A[3857] * x[1541];
  y[1542] += A[3858] * x[1543];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1543] += A[1 * i0 + 3859] * x[1 * i0 + 1542];
  }
  y[1544] += A[3862] * x[1543];
  y[1544] += A[3863] * x[1545];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1545] += A[1 * i0 + 3864] * x[1 * i0 + 1544];
  }
  y[1546] += A[3867] * x[1545];
  y[1546] += A[3868] * x[1547];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1547] += A[1 * i0 + 3869] * x[1 * i0 + 1546];
  }
  y[1548] += A[3872] * x[1547];
  y[1548] += A[3873] * x[1549];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1549] += A[1 * i0 + 3874] * x[1 * i0 + 1548];
  }
  y[1550] += A[3877] * x[1549];
  y[1550] += A[3878] * x[1551];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1551] += A[1 * i0 + 3879] * x[1 * i0 + 1550];
  }
  y[1552] += A[3882] * x[1551];
  y[1552] += A[3883] * x[1553];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1553] += A[1 * i0 + 3884] * x[1 * i0 + 1552];
  }
  y[1554] += A[3887] * x[1553];
  y[1554] += A[3888] * x[1555];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1555] += A[1 * i0 + 3889] * x[1 * i0 + 1554];
  }
  y[1556] += A[3892] * x[1555];
  y[1556] += A[3893] * x[1557];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1557] += A[1 * i0 + 3894] * x[1 * i0 + 1556];
  }
  y[1558] += A[3897] * x[1557];
  y[1558] += A[3898] * x[1559];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1559] += A[1 * i0 + 3899] * x[1 * i0 + 1558];
  }
  y[1560] += A[3902] * x[1559];
  y[1560] += A[3903] * x[1561];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1561] += A[1 * i0 + 3904] * x[1 * i0 + 1560];
  }
  y[1562] += A[3907] * x[1561];
  y[1562] += A[3908] * x[1563];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1563] += A[1 * i0 + 3909] * x[1 * i0 + 1562];
  }
  y[1564] += A[3912] * x[1563];
  y[1564] += A[3913] * x[1565];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1565] += A[1 * i0 + 3914] * x[1 * i0 + 1564];
  }
  y[1566] += A[3917] * x[1565];
  y[1566] += A[3918] * x[1567];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1567] += A[1 * i0 + 3919] * x[1 * i0 + 1566];
  }
  y[1568] += A[3922] * x[1567];
  y[1568] += A[3923] * x[1569];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1569] += A[1 * i0 + 3924] * x[1 * i0 + 1568];
  }
  y[1570] += A[3927] * x[1569];
  y[1570] += A[3928] * x[1571];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1571] += A[1 * i0 + 3929] * x[1 * i0 + 1570];
  }
  y[1572] += A[3932] * x[1571];
  y[1572] += A[3933] * x[1573];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1573] += A[1 * i0 + 3934] * x[1 * i0 + 1572];
  }
  y[1574] += A[3937] * x[1573];
  y[1574] += A[3938] * x[1575];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1575] += A[1 * i0 + 3939] * x[1 * i0 + 1574];
  }
  y[1576] += A[3942] * x[1575];
  y[1576] += A[3943] * x[1577];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1577] += A[1 * i0 + 3944] * x[1 * i0 + 1576];
  }
  y[1578] += A[3947] * x[1577];
  y[1578] += A[3948] * x[1579];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1579] += A[1 * i0 + 3949] * x[1 * i0 + 1578];
  }
  y[1580] += A[3952] * x[1579];
  y[1580] += A[3953] * x[1581];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1581] += A[1 * i0 + 3954] * x[1 * i0 + 1580];
  }
  y[1582] += A[3957] * x[1581];
  y[1582] += A[3958] * x[1583];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1583] += A[1 * i0 + 3959] * x[1 * i0 + 1582];
  }
  y[1584] += A[3962] * x[1583];
  y[1584] += A[3963] * x[1585];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1585] += A[1 * i0 + 3964] * x[1 * i0 + 1584];
  }
  y[1586] += A[3967] * x[1585];
  y[1586] += A[3968] * x[1587];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1587] += A[1 * i0 + 3969] * x[1 * i0 + 1586];
  }
  y[1588] += A[3972] * x[1587];
  y[1588] += A[3973] * x[1589];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1589] += A[1 * i0 + 3974] * x[1 * i0 + 1588];
  }
  y[1590] += A[3977] * x[1589];
  y[1590] += A[3978] * x[1591];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1591] += A[1 * i0 + 3979] * x[1 * i0 + 1590];
  }
  y[1592] += A[3982] * x[1591];
  y[1592] += A[3983] * x[1593];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1593] += A[1 * i0 + 3984] * x[1 * i0 + 1592];
  }
  y[1594] += A[3987] * x[1593];
  y[1594] += A[3988] * x[1595];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1595] += A[1 * i0 + 3989] * x[1 * i0 + 1594];
  }
  y[1596] += A[3992] * x[1595];
  y[1596] += A[3993] * x[1597];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1597] += A[1 * i0 + 3994] * x[1 * i0 + 1596];
  }
  y[1598] += A[3997] * x[1597];
  y[1598] += A[3998] * x[1599];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1599] += A[1 * i0 + 3999] * x[1 * i0 + 1598];
  }
  y[1600] += A[4002] * x[1599];
  y[1600] += A[4003] * x[1601];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1601] += A[1 * i0 + 4004] * x[1 * i0 + 1600];
  }
  y[1602] += A[4007] * x[1601];
  y[1602] += A[4008] * x[1603];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1603] += A[1 * i0 + 4009] * x[1 * i0 + 1602];
  }
  y[1604] += A[4012] * x[1603];
  y[1604] += A[4013] * x[1605];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1605] += A[1 * i0 + 4014] * x[1 * i0 + 1604];
  }
  y[1606] += A[4017] * x[1605];
  y[1606] += A[4018] * x[1607];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1607] += A[1 * i0 + 4019] * x[1 * i0 + 1606];
  }
  y[1608] += A[4022] * x[1607];
  y[1608] += A[4023] * x[1609];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1609] += A[1 * i0 + 4024] * x[1 * i0 + 1608];
  }
  y[1610] += A[4027] * x[1609];
  y[1610] += A[4028] * x[1611];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1611] += A[1 * i0 + 4029] * x[1 * i0 + 1610];
  }
  y[1612] += A[4032] * x[653];
  y[1612] += A[4033] * x[812];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 1612] += A[1 * i0 + 4034] * x[1 * i0 + 1611];
  }
  y[1612] += A[4036] * x[4016];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 1613] += A[1 * i0 + 4037] * x[1 * i0 + 1613];
  }
  y[1613] += A[4039] * x[1772];
  y[1614] += A[4040] * x[1613];
  y[1614] += A[4041] * x[1615];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1615] += A[1 * i0 + 4042] * x[1 * i0 + 1614];
  }
  y[1616] += A[4045] * x[1615];
  y[1616] += A[4046] * x[1617];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1617] += A[1 * i0 + 4047] * x[1 * i0 + 1616];
  }
  y[1618] += A[4050] * x[1617];
  y[1618] += A[4051] * x[1619];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1619] += A[1 * i0 + 4052] * x[1 * i0 + 1618];
  }
  y[1620] += A[4055] * x[1619];
  y[1620] += A[4056] * x[1621];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1621] += A[1 * i0 + 4057] * x[1 * i0 + 1620];
  }
  y[1622] += A[4060] * x[1621];
  y[1622] += A[4061] * x[1623];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1623] += A[1 * i0 + 4062] * x[1 * i0 + 1622];
  }
  y[1624] += A[4065] * x[1623];
  y[1624] += A[4066] * x[1625];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1625] += A[1 * i0 + 4067] * x[1 * i0 + 1624];
  }
  y[1626] += A[4070] * x[1625];
  y[1626] += A[4071] * x[1627];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1627] += A[1 * i0 + 4072] * x[1 * i0 + 1626];
  }
  y[1628] += A[4075] * x[1627];
  y[1628] += A[4076] * x[1629];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1629] += A[1 * i0 + 4077] * x[1 * i0 + 1628];
  }
  y[1630] += A[4080] * x[1629];
  y[1630] += A[4081] * x[1631];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1631] += A[1 * i0 + 4082] * x[1 * i0 + 1630];
  }
  y[1632] += A[4085] * x[1631];
  y[1632] += A[4086] * x[1633];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1633] += A[1 * i0 + 4087] * x[1 * i0 + 1632];
  }
  y[1634] += A[4090] * x[1633];
  y[1634] += A[4091] * x[1635];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1635] += A[1 * i0 + 4092] * x[1 * i0 + 1634];
  }
  y[1636] += A[4095] * x[1635];
  y[1636] += A[4096] * x[1637];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1637] += A[1 * i0 + 4097] * x[1 * i0 + 1636];
  }
  y[1638] += A[4100] * x[1637];
  y[1638] += A[4101] * x[1639];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1639] += A[1 * i0 + 4102] * x[1 * i0 + 1638];
  }
  y[1640] += A[4105] * x[1639];
  y[1640] += A[4106] * x[1641];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1641] += A[1 * i0 + 4107] * x[1 * i0 + 1640];
  }
  y[1642] += A[4110] * x[1641];
  y[1642] += A[4111] * x[1643];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1643] += A[1 * i0 + 4112] * x[1 * i0 + 1642];
  }
  y[1644] += A[4115] * x[1643];
  y[1644] += A[4116] * x[1645];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1645] += A[1 * i0 + 4117] * x[1 * i0 + 1644];
  }
  y[1646] += A[4120] * x[1645];
  y[1646] += A[4121] * x[1647];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1647] += A[1 * i0 + 4122] * x[1 * i0 + 1646];
  }
  y[1648] += A[4125] * x[1647];
  y[1648] += A[4126] * x[1649];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1649] += A[1 * i0 + 4127] * x[1 * i0 + 1648];
  }
  y[1650] += A[4130] * x[1649];
  y[1650] += A[4131] * x[1651];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1651] += A[1 * i0 + 4132] * x[1 * i0 + 1650];
  }
  y[1652] += A[4135] * x[1651];
  y[1652] += A[4136] * x[1653];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1653] += A[1 * i0 + 4137] * x[1 * i0 + 1652];
  }
  y[1654] += A[4140] * x[1653];
  y[1654] += A[4141] * x[1655];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1655] += A[1 * i0 + 4142] * x[1 * i0 + 1654];
  }
  y[1656] += A[4145] * x[1655];
  y[1656] += A[4146] * x[1657];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1657] += A[1 * i0 + 4147] * x[1 * i0 + 1656];
  }
  y[1658] += A[4150] * x[1657];
  y[1658] += A[4151] * x[1659];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1659] += A[1 * i0 + 4152] * x[1 * i0 + 1658];
  }
  y[1660] += A[4155] * x[1659];
  y[1660] += A[4156] * x[1661];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1661] += A[1 * i0 + 4157] * x[1 * i0 + 1660];
  }
  y[1662] += A[4160] * x[1661];
  y[1662] += A[4161] * x[1663];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1663] += A[1 * i0 + 4162] * x[1 * i0 + 1662];
  }
  y[1664] += A[4165] * x[1663];
  y[1664] += A[4166] * x[1665];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1665] += A[1 * i0 + 4167] * x[1 * i0 + 1664];
  }
  y[1666] += A[4170] * x[1665];
  y[1666] += A[4171] * x[1667];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1667] += A[1 * i0 + 4172] * x[1 * i0 + 1666];
  }
  y[1668] += A[4175] * x[1667];
  y[1668] += A[4176] * x[1669];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1669] += A[1 * i0 + 4177] * x[1 * i0 + 1668];
  }
  y[1670] += A[4180] * x[1669];
  y[1670] += A[4181] * x[1671];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1671] += A[1 * i0 + 4182] * x[1 * i0 + 1670];
  }
  y[1672] += A[4185] * x[1671];
  y[1672] += A[4186] * x[1673];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1673] += A[1 * i0 + 4187] * x[1 * i0 + 1672];
  }
  y[1674] += A[4190] * x[1673];
  y[1674] += A[4191] * x[1675];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1675] += A[1 * i0 + 4192] * x[1 * i0 + 1674];
  }
  y[1676] += A[4195] * x[1675];
  y[1676] += A[4196] * x[1677];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1677] += A[1 * i0 + 4197] * x[1 * i0 + 1676];
  }
  y[1678] += A[4200] * x[1677];
  y[1678] += A[4201] * x[1679];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1679] += A[1 * i0 + 4202] * x[1 * i0 + 1678];
  }
  y[1680] += A[4205] * x[1679];
  y[1680] += A[4206] * x[1681];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1681] += A[1 * i0 + 4207] * x[1 * i0 + 1680];
  }
  y[1682] += A[4210] * x[1681];
  y[1682] += A[4211] * x[1683];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1683] += A[1 * i0 + 4212] * x[1 * i0 + 1682];
  }
  y[1684] += A[4215] * x[1683];
  y[1684] += A[4216] * x[1685];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1685] += A[1 * i0 + 4217] * x[1 * i0 + 1684];
  }
  y[1686] += A[4220] * x[1685];
  y[1686] += A[4221] * x[1687];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1687] += A[1 * i0 + 4222] * x[1 * i0 + 1686];
  }
  y[1688] += A[4225] * x[1687];
  y[1688] += A[4226] * x[1689];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1689] += A[1 * i0 + 4227] * x[1 * i0 + 1688];
  }
  y[1690] += A[4230] * x[1689];
  y[1690] += A[4231] * x[1691];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1691] += A[1 * i0 + 4232] * x[1 * i0 + 1690];
  }
  y[1692] += A[4235] * x[1691];
  y[1692] += A[4236] * x[1693];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1693] += A[1 * i0 + 4237] * x[1 * i0 + 1692];
  }
  y[1694] += A[4240] * x[1693];
  y[1694] += A[4241] * x[1695];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1695] += A[1 * i0 + 4242] * x[1 * i0 + 1694];
  }
  y[1696] += A[4245] * x[1695];
  y[1696] += A[4246] * x[1697];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1697] += A[1 * i0 + 4247] * x[1 * i0 + 1696];
  }
  y[1698] += A[4250] * x[1697];
  y[1698] += A[4251] * x[1699];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1699] += A[1 * i0 + 4252] * x[1 * i0 + 1698];
  }
  y[1700] += A[4255] * x[1699];
  y[1700] += A[4256] * x[1701];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1701] += A[1 * i0 + 4257] * x[1 * i0 + 1700];
  }
  y[1702] += A[4260] * x[1701];
  y[1702] += A[4261] * x[1703];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1703] += A[1 * i0 + 4262] * x[1 * i0 + 1702];
  }
  y[1704] += A[4265] * x[1703];
  y[1704] += A[4266] * x[1705];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1705] += A[1 * i0 + 4267] * x[1 * i0 + 1704];
  }
  y[1706] += A[4270] * x[1705];
  y[1706] += A[4271] * x[1707];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1707] += A[1 * i0 + 4272] * x[1 * i0 + 1706];
  }
  y[1708] += A[4275] * x[1707];
  y[1708] += A[4276] * x[1709];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1709] += A[1 * i0 + 4277] * x[1 * i0 + 1708];
  }
  y[1710] += A[4280] * x[1709];
  y[1710] += A[4281] * x[1711];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1711] += A[1 * i0 + 4282] * x[1 * i0 + 1710];
  }
  y[1712] += A[4285] * x[1711];
  y[1712] += A[4286] * x[1713];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1713] += A[1 * i0 + 4287] * x[1 * i0 + 1712];
  }
  y[1714] += A[4290] * x[1713];
  y[1714] += A[4291] * x[1715];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1715] += A[1 * i0 + 4292] * x[1 * i0 + 1714];
  }
  y[1716] += A[4295] * x[1715];
  y[1716] += A[4296] * x[1717];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1717] += A[1 * i0 + 4297] * x[1 * i0 + 1716];
  }
  y[1718] += A[4300] * x[1717];
  y[1718] += A[4301] * x[1719];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1719] += A[1 * i0 + 4302] * x[1 * i0 + 1718];
  }
  y[1720] += A[4305] * x[1719];
  y[1720] += A[4306] * x[1721];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1721] += A[1 * i0 + 4307] * x[1 * i0 + 1720];
  }
  y[1722] += A[4310] * x[1721];
  y[1722] += A[4311] * x[1723];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1723] += A[1 * i0 + 4312] * x[1 * i0 + 1722];
  }
  y[1724] += A[4315] * x[1723];
  y[1724] += A[4316] * x[1725];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1725] += A[1 * i0 + 4317] * x[1 * i0 + 1724];
  }
  y[1726] += A[4320] * x[1725];
  y[1726] += A[4321] * x[1727];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1727] += A[1 * i0 + 4322] * x[1 * i0 + 1726];
  }
  y[1728] += A[4325] * x[1727];
  y[1728] += A[4326] * x[1729];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1729] += A[1 * i0 + 4327] * x[1 * i0 + 1728];
  }
  y[1730] += A[4330] * x[1729];
  y[1730] += A[4331] * x[1731];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1731] += A[1 * i0 + 4332] * x[1 * i0 + 1730];
  }
  y[1732] += A[4335] * x[1731];
  y[1732] += A[4336] * x[1733];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1733] += A[1 * i0 + 4337] * x[1 * i0 + 1732];
  }
  y[1734] += A[4340] * x[1733];
  y[1734] += A[4341] * x[1735];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1735] += A[1 * i0 + 4342] * x[1 * i0 + 1734];
  }
  y[1736] += A[4345] * x[1735];
  y[1736] += A[4346] * x[1737];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1737] += A[1 * i0 + 4347] * x[1 * i0 + 1736];
  }
  y[1738] += A[4350] * x[1737];
  y[1738] += A[4351] * x[1739];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1739] += A[1 * i0 + 4352] * x[1 * i0 + 1738];
  }
  y[1740] += A[4355] * x[1739];
  y[1740] += A[4356] * x[1741];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1741] += A[1 * i0 + 4357] * x[1 * i0 + 1740];
  }
  y[1742] += A[4360] * x[1741];
  y[1742] += A[4361] * x[1743];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1743] += A[1 * i0 + 4362] * x[1 * i0 + 1742];
  }
  y[1744] += A[4365] * x[1743];
  y[1744] += A[4366] * x[1745];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1745] += A[1 * i0 + 4367] * x[1 * i0 + 1744];
  }
  y[1746] += A[4370] * x[1745];
  y[1746] += A[4371] * x[1747];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1747] += A[1 * i0 + 4372] * x[1 * i0 + 1746];
  }
  y[1748] += A[4375] * x[1747];
  y[1748] += A[4376] * x[1749];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1749] += A[1 * i0 + 4377] * x[1 * i0 + 1748];
  }
  y[1750] += A[4380] * x[1749];
  y[1750] += A[4381] * x[1751];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1751] += A[1 * i0 + 4382] * x[1 * i0 + 1750];
  }
  y[1752] += A[4385] * x[1751];
  y[1752] += A[4386] * x[1753];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1753] += A[1 * i0 + 4387] * x[1 * i0 + 1752];
  }
  y[1754] += A[4390] * x[1753];
  y[1754] += A[4391] * x[1755];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1755] += A[1 * i0 + 4392] * x[1 * i0 + 1754];
  }
  y[1756] += A[4395] * x[1755];
  y[1756] += A[4396] * x[1757];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1757] += A[1 * i0 + 4397] * x[1 * i0 + 1756];
  }
  y[1758] += A[4400] * x[1757];
  y[1758] += A[4401] * x[1759];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1759] += A[1 * i0 + 4402] * x[1 * i0 + 1758];
  }
  y[1760] += A[4405] * x[1759];
  y[1760] += A[4406] * x[1761];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1761] += A[1 * i0 + 4407] * x[1 * i0 + 1760];
  }
  y[1762] += A[4410] * x[1761];
  y[1762] += A[4411] * x[1763];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1763] += A[1 * i0 + 4412] * x[1 * i0 + 1762];
  }
  y[1764] += A[4415] * x[1763];
  y[1764] += A[4416] * x[1765];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1765] += A[1 * i0 + 4417] * x[1 * i0 + 1764];
  }
  y[1766] += A[4420] * x[1765];
  y[1766] += A[4421] * x[1767];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1767] += A[1 * i0 + 4422] * x[1 * i0 + 1766];
  }
  y[1768] += A[4425] * x[1767];
  y[1768] += A[4426] * x[1769];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1769] += A[1 * i0 + 4427] * x[1 * i0 + 1768];
  }
  y[1770] += A[4430] * x[1769];
  y[1770] += A[4431] * x[1771];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 1771] += A[1 * i0 + 4432] * x[1 * i0 + 1770];
  }
  y[1771] += A[4434] * x[4012];
  y[1772] += A[4435] * x[7];
  y[1772] += A[4436] * x[1291];
  y[1772] += A[4437] * x[1613];
  y[1772] += A[4438] * x[1773];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1773] += A[1 * i0 + 4439] * x[1 * i0 + 1772];
  }
  y[1774] += A[4442] * x[1773];
  y[1774] += A[4443] * x[1775];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1775] += A[1 * i0 + 4444] * x[1 * i0 + 1774];
  }
  y[1776] += A[4447] * x[1775];
  y[1776] += A[4448] * x[1777];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1777] += A[1 * i0 + 4449] * x[1 * i0 + 1776];
  }
  y[1778] += A[4452] * x[1777];
  y[1778] += A[4453] * x[1779];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1779] += A[1 * i0 + 4454] * x[1 * i0 + 1778];
  }
  y[1780] += A[4457] * x[1779];
  y[1780] += A[4458] * x[1781];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1781] += A[1 * i0 + 4459] * x[1 * i0 + 1780];
  }
  y[1782] += A[4462] * x[1781];
  y[1782] += A[4463] * x[1783];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1783] += A[1 * i0 + 4464] * x[1 * i0 + 1782];
  }
  y[1784] += A[4467] * x[1783];
  y[1784] += A[4468] * x[1785];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1785] += A[1 * i0 + 4469] * x[1 * i0 + 1784];
  }
  y[1786] += A[4472] * x[1785];
  y[1786] += A[4473] * x[1787];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1787] += A[1 * i0 + 4474] * x[1 * i0 + 1786];
  }
  y[1788] += A[4477] * x[1787];
  y[1788] += A[4478] * x[1789];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1789] += A[1 * i0 + 4479] * x[1 * i0 + 1788];
  }
  y[1790] += A[4482] * x[1789];
  y[1790] += A[4483] * x[1791];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1791] += A[1 * i0 + 4484] * x[1 * i0 + 1790];
  }
  y[1792] += A[4487] * x[1791];
  y[1792] += A[4488] * x[1793];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1793] += A[1 * i0 + 4489] * x[1 * i0 + 1792];
  }
  y[1794] += A[4492] * x[1793];
  y[1794] += A[4493] * x[1795];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1795] += A[1 * i0 + 4494] * x[1 * i0 + 1794];
  }
  y[1796] += A[4497] * x[1795];
  y[1796] += A[4498] * x[1797];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1797] += A[1 * i0 + 4499] * x[1 * i0 + 1796];
  }
  y[1798] += A[4502] * x[1797];
  y[1798] += A[4503] * x[1799];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1799] += A[1 * i0 + 4504] * x[1 * i0 + 1798];
  }
  y[1800] += A[4507] * x[1799];
  y[1800] += A[4508] * x[1801];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1801] += A[1 * i0 + 4509] * x[1 * i0 + 1800];
  }
  y[1802] += A[4512] * x[1801];
  y[1802] += A[4513] * x[1803];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1803] += A[1 * i0 + 4514] * x[1 * i0 + 1802];
  }
  y[1804] += A[4517] * x[1803];
  y[1804] += A[4518] * x[1805];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1805] += A[1 * i0 + 4519] * x[1 * i0 + 1804];
  }
  y[1806] += A[4522] * x[1805];
  y[1806] += A[4523] * x[1807];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1807] += A[1 * i0 + 4524] * x[1 * i0 + 1806];
  }
  y[1808] += A[4527] * x[1807];
  y[1808] += A[4528] * x[1809];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1809] += A[1 * i0 + 4529] * x[1 * i0 + 1808];
  }
  y[1810] += A[4532] * x[1809];
  y[1810] += A[4533] * x[1811];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1811] += A[1 * i0 + 4534] * x[1 * i0 + 1810];
  }
  y[1812] += A[4537] * x[1811];
  y[1812] += A[4538] * x[1813];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1813] += A[1 * i0 + 4539] * x[1 * i0 + 1812];
  }
  y[1814] += A[4542] * x[1813];
  y[1814] += A[4543] * x[1815];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1815] += A[1 * i0 + 4544] * x[1 * i0 + 1814];
  }
  y[1816] += A[4547] * x[1815];
  y[1816] += A[4548] * x[1817];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1817] += A[1 * i0 + 4549] * x[1 * i0 + 1816];
  }
  y[1818] += A[4552] * x[1817];
  y[1818] += A[4553] * x[1819];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1819] += A[1 * i0 + 4554] * x[1 * i0 + 1818];
  }
  y[1820] += A[4557] * x[1819];
  y[1820] += A[4558] * x[1821];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1821] += A[1 * i0 + 4559] * x[1 * i0 + 1820];
  }
  y[1822] += A[4562] * x[1821];
  y[1822] += A[4563] * x[1823];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1823] += A[1 * i0 + 4564] * x[1 * i0 + 1822];
  }
  y[1824] += A[4567] * x[1823];
  y[1824] += A[4568] * x[1825];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1825] += A[1 * i0 + 4569] * x[1 * i0 + 1824];
  }
  y[1826] += A[4572] * x[1825];
  y[1826] += A[4573] * x[1827];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1827] += A[1 * i0 + 4574] * x[1 * i0 + 1826];
  }
  y[1828] += A[4577] * x[1827];
  y[1828] += A[4578] * x[1829];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1829] += A[1 * i0 + 4579] * x[1 * i0 + 1828];
  }
  y[1830] += A[4582] * x[1829];
  y[1830] += A[4583] * x[1831];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1831] += A[1 * i0 + 4584] * x[1 * i0 + 1830];
  }
  y[1832] += A[4587] * x[1831];
  y[1832] += A[4588] * x[1833];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1833] += A[1 * i0 + 4589] * x[1 * i0 + 1832];
  }
  y[1834] += A[4592] * x[1833];
  y[1834] += A[4593] * x[1835];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1835] += A[1 * i0 + 4594] * x[1 * i0 + 1834];
  }
  y[1836] += A[4597] * x[1835];
  y[1836] += A[4598] * x[1837];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1837] += A[1 * i0 + 4599] * x[1 * i0 + 1836];
  }
  y[1838] += A[4602] * x[1837];
  y[1838] += A[4603] * x[1839];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1839] += A[1 * i0 + 4604] * x[1 * i0 + 1838];
  }
  y[1840] += A[4607] * x[1839];
  y[1840] += A[4608] * x[1841];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1841] += A[1 * i0 + 4609] * x[1 * i0 + 1840];
  }
  y[1842] += A[4612] * x[1841];
  y[1842] += A[4613] * x[1843];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1843] += A[1 * i0 + 4614] * x[1 * i0 + 1842];
  }
  y[1844] += A[4617] * x[1843];
  y[1844] += A[4618] * x[1845];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1845] += A[1 * i0 + 4619] * x[1 * i0 + 1844];
  }
  y[1846] += A[4622] * x[1845];
  y[1846] += A[4623] * x[1847];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1847] += A[1 * i0 + 4624] * x[1 * i0 + 1846];
  }
  y[1848] += A[4627] * x[1847];
  y[1848] += A[4628] * x[1849];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1849] += A[1 * i0 + 4629] * x[1 * i0 + 1848];
  }
  y[1850] += A[4632] * x[1849];
  y[1850] += A[4633] * x[1851];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1851] += A[1 * i0 + 4634] * x[1 * i0 + 1850];
  }
  y[1852] += A[4637] * x[1851];
  y[1852] += A[4638] * x[1853];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1853] += A[1 * i0 + 4639] * x[1 * i0 + 1852];
  }
  y[1854] += A[4642] * x[1853];
  y[1854] += A[4643] * x[1855];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1855] += A[1 * i0 + 4644] * x[1 * i0 + 1854];
  }
  y[1856] += A[4647] * x[1855];
  y[1856] += A[4648] * x[1857];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1857] += A[1 * i0 + 4649] * x[1 * i0 + 1856];
  }
  y[1858] += A[4652] * x[1857];
  y[1858] += A[4653] * x[1859];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1859] += A[1 * i0 + 4654] * x[1 * i0 + 1858];
  }
  y[1860] += A[4657] * x[1859];
  y[1860] += A[4658] * x[1861];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1861] += A[1 * i0 + 4659] * x[1 * i0 + 1860];
  }
  y[1862] += A[4662] * x[1861];
  y[1862] += A[4663] * x[1863];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1863] += A[1 * i0 + 4664] * x[1 * i0 + 1862];
  }
  y[1864] += A[4667] * x[1863];
  y[1864] += A[4668] * x[1865];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1865] += A[1 * i0 + 4669] * x[1 * i0 + 1864];
  }
  y[1866] += A[4672] * x[1865];
  y[1866] += A[4673] * x[1867];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1867] += A[1 * i0 + 4674] * x[1 * i0 + 1866];
  }
  y[1868] += A[4677] * x[1867];
  y[1868] += A[4678] * x[1869];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1869] += A[1 * i0 + 4679] * x[1 * i0 + 1868];
  }
  y[1870] += A[4682] * x[1869];
  y[1870] += A[4683] * x[1871];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1871] += A[1 * i0 + 4684] * x[1 * i0 + 1870];
  }
  y[1872] += A[4687] * x[1871];
  y[1872] += A[4688] * x[1873];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1873] += A[1 * i0 + 4689] * x[1 * i0 + 1872];
  }
  y[1874] += A[4692] * x[1873];
  y[1874] += A[4693] * x[1875];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1875] += A[1 * i0 + 4694] * x[1 * i0 + 1874];
  }
  y[1876] += A[4697] * x[1875];
  y[1876] += A[4698] * x[1877];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1877] += A[1 * i0 + 4699] * x[1 * i0 + 1876];
  }
  y[1878] += A[4702] * x[1877];
  y[1878] += A[4703] * x[1879];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1879] += A[1 * i0 + 4704] * x[1 * i0 + 1878];
  }
  y[1880] += A[4707] * x[1879];
  y[1880] += A[4708] * x[1881];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1881] += A[1 * i0 + 4709] * x[1 * i0 + 1880];
  }
  y[1882] += A[4712] * x[1881];
  y[1882] += A[4713] * x[1883];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1883] += A[1 * i0 + 4714] * x[1 * i0 + 1882];
  }
  y[1884] += A[4717] * x[1883];
  y[1884] += A[4718] * x[1885];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1885] += A[1 * i0 + 4719] * x[1 * i0 + 1884];
  }
  y[1886] += A[4722] * x[1885];
  y[1886] += A[4723] * x[1887];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1887] += A[1 * i0 + 4724] * x[1 * i0 + 1886];
  }
  y[1888] += A[4727] * x[1887];
  y[1888] += A[4728] * x[1889];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1889] += A[1 * i0 + 4729] * x[1 * i0 + 1888];
  }
  y[1890] += A[4732] * x[1889];
  y[1890] += A[4733] * x[1891];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1891] += A[1 * i0 + 4734] * x[1 * i0 + 1890];
  }
  y[1892] += A[4737] * x[1891];
  y[1892] += A[4738] * x[1893];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1893] += A[1 * i0 + 4739] * x[1 * i0 + 1892];
  }
  y[1894] += A[4742] * x[1893];
  y[1894] += A[4743] * x[1895];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1895] += A[1 * i0 + 4744] * x[1 * i0 + 1894];
  }
  y[1896] += A[4747] * x[1895];
  y[1896] += A[4748] * x[1897];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1897] += A[1 * i0 + 4749] * x[1 * i0 + 1896];
  }
  y[1898] += A[4752] * x[1897];
  y[1898] += A[4753] * x[1899];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1899] += A[1 * i0 + 4754] * x[1 * i0 + 1898];
  }
  y[1900] += A[4757] * x[1899];
  y[1900] += A[4758] * x[1901];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1901] += A[1 * i0 + 4759] * x[1 * i0 + 1900];
  }
  y[1902] += A[4762] * x[1901];
  y[1902] += A[4763] * x[1903];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1903] += A[1 * i0 + 4764] * x[1 * i0 + 1902];
  }
  y[1904] += A[4767] * x[1903];
  y[1904] += A[4768] * x[1905];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1905] += A[1 * i0 + 4769] * x[1 * i0 + 1904];
  }
  y[1906] += A[4772] * x[1905];
  y[1906] += A[4773] * x[1907];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1907] += A[1 * i0 + 4774] * x[1 * i0 + 1906];
  }
  y[1908] += A[4777] * x[1907];
  y[1908] += A[4778] * x[1909];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1909] += A[1 * i0 + 4779] * x[1 * i0 + 1908];
  }
  y[1910] += A[4782] * x[1909];
  y[1910] += A[4783] * x[1911];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1911] += A[1 * i0 + 4784] * x[1 * i0 + 1910];
  }
  y[1912] += A[4787] * x[1911];
  y[1912] += A[4788] * x[1913];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1913] += A[1 * i0 + 4789] * x[1 * i0 + 1912];
  }
  y[1914] += A[4792] * x[1913];
  y[1914] += A[4793] * x[1915];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1915] += A[1 * i0 + 4794] * x[1 * i0 + 1914];
  }
  y[1916] += A[4797] * x[1915];
  y[1916] += A[4798] * x[1917];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1917] += A[1 * i0 + 4799] * x[1 * i0 + 1916];
  }
  y[1918] += A[4802] * x[1917];
  y[1918] += A[4803] * x[1919];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1919] += A[1 * i0 + 4804] * x[1 * i0 + 1918];
  }
  y[1920] += A[4807] * x[1919];
  y[1920] += A[4808] * x[1921];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1921] += A[1 * i0 + 4809] * x[1 * i0 + 1920];
  }
  y[1922] += A[4812] * x[1921];
  y[1922] += A[4813] * x[1923];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1923] += A[1 * i0 + 4814] * x[1 * i0 + 1922];
  }
  y[1924] += A[4817] * x[1923];
  y[1924] += A[4818] * x[1925];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1925] += A[1 * i0 + 4819] * x[1 * i0 + 1924];
  }
  y[1926] += A[4822] * x[1925];
  y[1926] += A[4823] * x[1927];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1927] += A[1 * i0 + 4824] * x[1 * i0 + 1926];
  }
  y[1928] += A[4827] * x[1927];
  y[1928] += A[4828] * x[1929];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1929] += A[1 * i0 + 4829] * x[1 * i0 + 1928];
  }
  y[1930] += A[4832] * x[1929];
  y[1930] += A[4833] * x[1931];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1931] += A[1 * i0 + 4834] * x[1 * i0 + 1930];
  }
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1932] += A[1 * i0 + 4837] * x[1 * i0 + 1931];
  }
  y[1932] += A[4840] * x[4025];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1933] += A[1 * i0 + 4841] * x[1 * i0 + 1932];
  }
  y[1934] += A[4844] * x[1933];
  y[1934] += A[4845] * x[1935];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1935] += A[1 * i0 + 4846] * x[1 * i0 + 1934];
  }
  y[1936] += A[4849] * x[1935];
  y[1936] += A[4850] * x[1937];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1937] += A[1 * i0 + 4851] * x[1 * i0 + 1936];
  }
  y[1938] += A[4854] * x[1937];
  y[1938] += A[4855] * x[1939];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1939] += A[1 * i0 + 4856] * x[1 * i0 + 1938];
  }
  y[1940] += A[4859] * x[1939];
  y[1940] += A[4860] * x[1941];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1941] += A[1 * i0 + 4861] * x[1 * i0 + 1940];
  }
  y[1942] += A[4864] * x[1941];
  y[1942] += A[4865] * x[1943];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1943] += A[1 * i0 + 4866] * x[1 * i0 + 1942];
  }
  y[1944] += A[4869] * x[1943];
  y[1944] += A[4870] * x[1945];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1945] += A[1 * i0 + 4871] * x[1 * i0 + 1944];
  }
  y[1946] += A[4874] * x[1945];
  y[1946] += A[4875] * x[1947];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1947] += A[1 * i0 + 4876] * x[1 * i0 + 1946];
  }
  y[1948] += A[4879] * x[1947];
  y[1948] += A[4880] * x[1949];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1949] += A[1 * i0 + 4881] * x[1 * i0 + 1948];
  }
  y[1950] += A[4884] * x[1949];
  y[1950] += A[4885] * x[1951];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1951] += A[1 * i0 + 4886] * x[1 * i0 + 1950];
  }
  y[1952] += A[4889] * x[1951];
  y[1952] += A[4890] * x[1953];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1953] += A[1 * i0 + 4891] * x[1 * i0 + 1952];
  }
  y[1954] += A[4894] * x[1953];
  y[1954] += A[4895] * x[1955];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1955] += A[1 * i0 + 4896] * x[1 * i0 + 1954];
  }
  y[1956] += A[4899] * x[1955];
  y[1956] += A[4900] * x[1957];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1957] += A[1 * i0 + 4901] * x[1 * i0 + 1956];
  }
  y[1958] += A[4904] * x[1957];
  y[1958] += A[4905] * x[1959];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1959] += A[1 * i0 + 4906] * x[1 * i0 + 1958];
  }
  y[1960] += A[4909] * x[1959];
  y[1960] += A[4910] * x[1961];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1961] += A[1 * i0 + 4911] * x[1 * i0 + 1960];
  }
  y[1962] += A[4914] * x[1961];
  y[1962] += A[4915] * x[1963];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1963] += A[1 * i0 + 4916] * x[1 * i0 + 1962];
  }
  y[1964] += A[4919] * x[1963];
  y[1964] += A[4920] * x[1965];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1965] += A[1 * i0 + 4921] * x[1 * i0 + 1964];
  }
  y[1966] += A[4924] * x[1965];
  y[1966] += A[4925] * x[1967];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1967] += A[1 * i0 + 4926] * x[1 * i0 + 1966];
  }
  y[1968] += A[4929] * x[1967];
  y[1968] += A[4930] * x[1969];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1969] += A[1 * i0 + 4931] * x[1 * i0 + 1968];
  }
  y[1970] += A[4934] * x[1969];
  y[1970] += A[4935] * x[1971];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1971] += A[1 * i0 + 4936] * x[1 * i0 + 1970];
  }
  y[1972] += A[4939] * x[1971];
  y[1972] += A[4940] * x[1973];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1973] += A[1 * i0 + 4941] * x[1 * i0 + 1972];
  }
  y[1974] += A[4944] * x[1973];
  y[1974] += A[4945] * x[1975];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1975] += A[1 * i0 + 4946] * x[1 * i0 + 1974];
  }
  y[1976] += A[4949] * x[1975];
  y[1976] += A[4950] * x[1977];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1977] += A[1 * i0 + 4951] * x[1 * i0 + 1976];
  }
  y[1978] += A[4954] * x[1977];
  y[1978] += A[4955] * x[1979];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1979] += A[1 * i0 + 4956] * x[1 * i0 + 1978];
  }
  y[1980] += A[4959] * x[1979];
  y[1980] += A[4960] * x[1981];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1981] += A[1 * i0 + 4961] * x[1 * i0 + 1980];
  }
  y[1982] += A[4964] * x[1981];
  y[1982] += A[4965] * x[1983];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1983] += A[1 * i0 + 4966] * x[1 * i0 + 1982];
  }
  y[1984] += A[4969] * x[1983];
  y[1984] += A[4970] * x[1985];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1985] += A[1 * i0 + 4971] * x[1 * i0 + 1984];
  }
  y[1986] += A[4974] * x[1985];
  y[1986] += A[4975] * x[1987];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1987] += A[1 * i0 + 4976] * x[1 * i0 + 1986];
  }
  y[1988] += A[4979] * x[1987];
  y[1988] += A[4980] * x[1989];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1989] += A[1 * i0 + 4981] * x[1 * i0 + 1988];
  }
  y[1990] += A[4984] * x[1989];
  y[1990] += A[4985] * x[1991];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1991] += A[1 * i0 + 4986] * x[1 * i0 + 1990];
  }
  y[1992] += A[4989] * x[1991];
  y[1992] += A[4990] * x[1993];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1993] += A[1 * i0 + 4991] * x[1 * i0 + 1992];
  }
  y[1994] += A[4994] * x[1993];
  y[1994] += A[4995] * x[1995];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1995] += A[1 * i0 + 4996] * x[1 * i0 + 1994];
  }
  y[1996] += A[4999] * x[1995];
  y[1996] += A[5000] * x[1997];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1997] += A[1 * i0 + 5001] * x[1 * i0 + 1996];
  }
  y[1998] += A[5004] * x[1997];
  y[1998] += A[5005] * x[1999];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 1999] += A[1 * i0 + 5006] * x[1 * i0 + 1998];
  }
  y[2000] += A[5009] * x[1999];
  y[2000] += A[5010] * x[2001];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2001] += A[1 * i0 + 5011] * x[1 * i0 + 2000];
  }
  y[2002] += A[5014] * x[2001];
  y[2002] += A[5015] * x[2003];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2003] += A[1 * i0 + 5016] * x[1 * i0 + 2002];
  }
  y[2004] += A[5019] * x[2003];
  y[2004] += A[5020] * x[2005];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2005] += A[1 * i0 + 5021] * x[1 * i0 + 2004];
  }
  y[2006] += A[5024] * x[2005];
  y[2006] += A[5025] * x[2007];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2007] += A[1 * i0 + 5026] * x[1 * i0 + 2006];
  }
  y[2008] += A[5029] * x[2007];
  y[2008] += A[5030] * x[2009];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2009] += A[1 * i0 + 5031] * x[1 * i0 + 2008];
  }
  y[2010] += A[5034] * x[2009];
  y[2010] += A[5035] * x[2011];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2011] += A[1 * i0 + 5036] * x[1 * i0 + 2010];
  }
  y[2012] += A[5039] * x[2011];
  y[2012] += A[5040] * x[2013];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2013] += A[1 * i0 + 5041] * x[1 * i0 + 2012];
  }
  y[2014] += A[5044] * x[2013];
  y[2014] += A[5045] * x[2015];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2015] += A[1 * i0 + 5046] * x[1 * i0 + 2014];
  }
  y[2016] += A[5049] * x[2015];
  y[2016] += A[5050] * x[2017];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2017] += A[1 * i0 + 5051] * x[1 * i0 + 2016];
  }
  y[2018] += A[5054] * x[2017];
  y[2018] += A[5055] * x[2019];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2019] += A[1 * i0 + 5056] * x[1 * i0 + 2018];
  }
  y[2020] += A[5059] * x[2019];
  y[2020] += A[5060] * x[2021];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2021] += A[1 * i0 + 5061] * x[1 * i0 + 2020];
  }
}

void kernel_spmv_fragment_4(double *restrict A, double *restrict x,
                            double *restrict y) {
  register int i0, i1, i2, i3, i4, i5, i6;

  y[2022] += A[5064] * x[2021];
  y[2022] += A[5065] * x[2023];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2023] += A[1 * i0 + 5066] * x[1 * i0 + 2022];
  }
  y[2024] += A[5069] * x[2023];
  y[2024] += A[5070] * x[2025];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2025] += A[1 * i0 + 5071] * x[1 * i0 + 2024];
  }
  y[2026] += A[5074] * x[2025];
  y[2026] += A[5075] * x[2027];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2027] += A[1 * i0 + 5076] * x[1 * i0 + 2026];
  }
  y[2028] += A[5079] * x[2027];
  y[2028] += A[5080] * x[2029];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2029] += A[1 * i0 + 5081] * x[1 * i0 + 2028];
  }
  y[2030] += A[5084] * x[2029];
  y[2030] += A[5085] * x[2031];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2031] += A[1 * i0 + 5086] * x[1 * i0 + 2030];
  }
  y[2032] += A[5089] * x[2031];
  y[2032] += A[5090] * x[2033];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2033] += A[1 * i0 + 5091] * x[1 * i0 + 2032];
  }
  y[2034] += A[5094] * x[2033];
  y[2034] += A[5095] * x[2035];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2035] += A[1 * i0 + 5096] * x[1 * i0 + 2034];
  }
  y[2036] += A[5099] * x[2035];
  y[2036] += A[5100] * x[2037];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2037] += A[1 * i0 + 5101] * x[1 * i0 + 2036];
  }
  y[2038] += A[5104] * x[2037];
  y[2038] += A[5105] * x[2039];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2039] += A[1 * i0 + 5106] * x[1 * i0 + 2038];
  }
  y[2040] += A[5109] * x[2039];
  y[2040] += A[5110] * x[2041];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2041] += A[1 * i0 + 5111] * x[1 * i0 + 2040];
  }
  y[2042] += A[5114] * x[2041];
  y[2042] += A[5115] * x[2043];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2043] += A[1 * i0 + 5116] * x[1 * i0 + 2042];
  }
  y[2044] += A[5119] * x[2043];
  y[2044] += A[5120] * x[2045];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2045] += A[1 * i0 + 5121] * x[1 * i0 + 2044];
  }
  y[2046] += A[5124] * x[2045];
  y[2046] += A[5125] * x[2047];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2047] += A[1 * i0 + 5126] * x[1 * i0 + 2046];
  }
  y[2048] += A[5129] * x[2047];
  y[2048] += A[5130] * x[2049];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2049] += A[1 * i0 + 5131] * x[1 * i0 + 2048];
  }
  y[2050] += A[5134] * x[2049];
  y[2050] += A[5135] * x[2051];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2051] += A[1 * i0 + 5136] * x[1 * i0 + 2050];
  }
  y[2052] += A[5139] * x[2051];
  y[2052] += A[5140] * x[2053];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2053] += A[1 * i0 + 5141] * x[1 * i0 + 2052];
  }
  y[2054] += A[5144] * x[2053];
  y[2054] += A[5145] * x[2055];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2055] += A[1 * i0 + 5146] * x[1 * i0 + 2054];
  }
  y[2056] += A[5149] * x[2055];
  y[2056] += A[5150] * x[2057];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2057] += A[1 * i0 + 5151] * x[1 * i0 + 2056];
  }
  y[2058] += A[5154] * x[2057];
  y[2058] += A[5155] * x[2059];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2059] += A[1 * i0 + 5156] * x[1 * i0 + 2058];
  }
  y[2060] += A[5159] * x[2059];
  y[2060] += A[5160] * x[2061];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2061] += A[1 * i0 + 5161] * x[1 * i0 + 2060];
  }
  y[2062] += A[5164] * x[2061];
  y[2062] += A[5165] * x[2063];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2063] += A[1 * i0 + 5166] * x[1 * i0 + 2062];
  }
  y[2064] += A[5169] * x[2063];
  y[2064] += A[5170] * x[2065];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2065] += A[1 * i0 + 5171] * x[1 * i0 + 2064];
  }
  y[2066] += A[5174] * x[2065];
  y[2066] += A[5175] * x[2067];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2067] += A[1 * i0 + 5176] * x[1 * i0 + 2066];
  }
  y[2068] += A[5179] * x[2067];
  y[2068] += A[5180] * x[2069];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2069] += A[1 * i0 + 5181] * x[1 * i0 + 2068];
  }
  y[2070] += A[5184] * x[2069];
  y[2070] += A[5185] * x[2071];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2071] += A[1 * i0 + 5186] * x[1 * i0 + 2070];
  }
  y[2072] += A[5189] * x[2071];
  y[2072] += A[5190] * x[2073];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2073] += A[1 * i0 + 5191] * x[1 * i0 + 2072];
  }
  y[2074] += A[5194] * x[2073];
  y[2074] += A[5195] * x[2075];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2075] += A[1 * i0 + 5196] * x[1 * i0 + 2074];
  }
  y[2076] += A[5199] * x[2075];
  y[2076] += A[5200] * x[2077];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2077] += A[1 * i0 + 5201] * x[1 * i0 + 2076];
  }
  y[2078] += A[5204] * x[2077];
  y[2078] += A[5205] * x[2079];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2079] += A[1 * i0 + 5206] * x[1 * i0 + 2078];
  }
  y[2080] += A[5209] * x[2079];
  y[2080] += A[5210] * x[2081];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2081] += A[1 * i0 + 5211] * x[1 * i0 + 2080];
  }
  y[2082] += A[5214] * x[2081];
  y[2082] += A[5215] * x[2083];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2083] += A[1 * i0 + 5216] * x[1 * i0 + 2082];
  }
  y[2084] += A[5219] * x[2083];
  y[2084] += A[5220] * x[2085];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2085] += A[1 * i0 + 5221] * x[1 * i0 + 2084];
  }
  y[2086] += A[5224] * x[2085];
  y[2086] += A[5225] * x[2087];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2087] += A[1 * i0 + 5226] * x[1 * i0 + 2086];
  }
  y[2088] += A[5229] * x[2087];
  y[2088] += A[5230] * x[2089];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2089] += A[1 * i0 + 5231] * x[1 * i0 + 2088];
  }
  y[2090] += A[5234] * x[2089];
  y[2090] += A[5235] * x[2091];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 2091] += A[1 * i0 + 5236] * x[1 * i0 + 2090];
  }
  y[2091] += A[5238] * x[4012];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 2092] += A[1 * i0 + 5239] * x[1 * i0 + 2092];
  }
  y[2092] += A[5241] * x[2251];
  y[2093] += A[5242] * x[2092];
  y[2093] += A[5243] * x[2094];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2094] += A[1 * i0 + 5244] * x[1 * i0 + 2093];
  }
  y[2095] += A[5247] * x[2094];
  y[2095] += A[5248] * x[2096];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2096] += A[1 * i0 + 5249] * x[1 * i0 + 2095];
  }
  y[2097] += A[5252] * x[2096];
  y[2097] += A[5253] * x[2098];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2098] += A[1 * i0 + 5254] * x[1 * i0 + 2097];
  }
  y[2099] += A[5257] * x[2098];
  y[2099] += A[5258] * x[2100];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2100] += A[1 * i0 + 5259] * x[1 * i0 + 2099];
  }
  y[2101] += A[5262] * x[2100];
  y[2101] += A[5263] * x[2102];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2102] += A[1 * i0 + 5264] * x[1 * i0 + 2101];
  }
  y[2103] += A[5267] * x[2102];
  y[2103] += A[5268] * x[2104];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2104] += A[1 * i0 + 5269] * x[1 * i0 + 2103];
  }
  y[2105] += A[5272] * x[2104];
  y[2105] += A[5273] * x[2106];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2106] += A[1 * i0 + 5274] * x[1 * i0 + 2105];
  }
  y[2107] += A[5277] * x[2106];
  y[2107] += A[5278] * x[2108];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2108] += A[1 * i0 + 5279] * x[1 * i0 + 2107];
  }
  y[2109] += A[5282] * x[2108];
  y[2109] += A[5283] * x[2110];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2110] += A[1 * i0 + 5284] * x[1 * i0 + 2109];
  }
  y[2111] += A[5287] * x[2110];
  y[2111] += A[5288] * x[2112];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2112] += A[1 * i0 + 5289] * x[1 * i0 + 2111];
  }
  y[2113] += A[5292] * x[2112];
  y[2113] += A[5293] * x[2114];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2114] += A[1 * i0 + 5294] * x[1 * i0 + 2113];
  }
  y[2115] += A[5297] * x[2114];
  y[2115] += A[5298] * x[2116];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2116] += A[1 * i0 + 5299] * x[1 * i0 + 2115];
  }
  y[2117] += A[5302] * x[2116];
  y[2117] += A[5303] * x[2118];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2118] += A[1 * i0 + 5304] * x[1 * i0 + 2117];
  }
  y[2119] += A[5307] * x[2118];
  y[2119] += A[5308] * x[2120];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2120] += A[1 * i0 + 5309] * x[1 * i0 + 2119];
  }
  y[2121] += A[5312] * x[2120];
  y[2121] += A[5313] * x[2122];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2122] += A[1 * i0 + 5314] * x[1 * i0 + 2121];
  }
  y[2123] += A[5317] * x[2122];
  y[2123] += A[5318] * x[2124];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2124] += A[1 * i0 + 5319] * x[1 * i0 + 2123];
  }
  y[2125] += A[5322] * x[2124];
  y[2125] += A[5323] * x[2126];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2126] += A[1 * i0 + 5324] * x[1 * i0 + 2125];
  }
  y[2127] += A[5327] * x[2126];
  y[2127] += A[5328] * x[2128];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2128] += A[1 * i0 + 5329] * x[1 * i0 + 2127];
  }
  y[2129] += A[5332] * x[2128];
  y[2129] += A[5333] * x[2130];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2130] += A[1 * i0 + 5334] * x[1 * i0 + 2129];
  }
  y[2131] += A[5337] * x[2130];
  y[2131] += A[5338] * x[2132];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2132] += A[1 * i0 + 5339] * x[1 * i0 + 2131];
  }
  y[2133] += A[5342] * x[2132];
  y[2133] += A[5343] * x[2134];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2134] += A[1 * i0 + 5344] * x[1 * i0 + 2133];
  }
  y[2135] += A[5347] * x[2134];
  y[2135] += A[5348] * x[2136];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2136] += A[1 * i0 + 5349] * x[1 * i0 + 2135];
  }
  y[2137] += A[5352] * x[2136];
  y[2137] += A[5353] * x[2138];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2138] += A[1 * i0 + 5354] * x[1 * i0 + 2137];
  }
  y[2139] += A[5357] * x[2138];
  y[2139] += A[5358] * x[2140];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2140] += A[1 * i0 + 5359] * x[1 * i0 + 2139];
  }
  y[2141] += A[5362] * x[2140];
  y[2141] += A[5363] * x[2142];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2142] += A[1 * i0 + 5364] * x[1 * i0 + 2141];
  }
  y[2143] += A[5367] * x[2142];
  y[2143] += A[5368] * x[2144];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2144] += A[1 * i0 + 5369] * x[1 * i0 + 2143];
  }
  y[2145] += A[5372] * x[2144];
  y[2145] += A[5373] * x[2146];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2146] += A[1 * i0 + 5374] * x[1 * i0 + 2145];
  }
  y[2147] += A[5377] * x[2146];
  y[2147] += A[5378] * x[2148];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2148] += A[1 * i0 + 5379] * x[1 * i0 + 2147];
  }
  y[2149] += A[5382] * x[2148];
  y[2149] += A[5383] * x[2150];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2150] += A[1 * i0 + 5384] * x[1 * i0 + 2149];
  }
  y[2151] += A[5387] * x[2150];
  y[2151] += A[5388] * x[2152];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2152] += A[1 * i0 + 5389] * x[1 * i0 + 2151];
  }
  y[2153] += A[5392] * x[2152];
  y[2153] += A[5393] * x[2154];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2154] += A[1 * i0 + 5394] * x[1 * i0 + 2153];
  }
  y[2155] += A[5397] * x[2154];
  y[2155] += A[5398] * x[2156];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2156] += A[1 * i0 + 5399] * x[1 * i0 + 2155];
  }
  y[2157] += A[5402] * x[2156];
  y[2157] += A[5403] * x[2158];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2158] += A[1 * i0 + 5404] * x[1 * i0 + 2157];
  }
  y[2159] += A[5407] * x[2158];
  y[2159] += A[5408] * x[2160];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2160] += A[1 * i0 + 5409] * x[1 * i0 + 2159];
  }
  y[2161] += A[5412] * x[2160];
  y[2161] += A[5413] * x[2162];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2162] += A[1 * i0 + 5414] * x[1 * i0 + 2161];
  }
  y[2163] += A[5417] * x[2162];
  y[2163] += A[5418] * x[2164];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2164] += A[1 * i0 + 5419] * x[1 * i0 + 2163];
  }
  y[2165] += A[5422] * x[2164];
  y[2165] += A[5423] * x[2166];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2166] += A[1 * i0 + 5424] * x[1 * i0 + 2165];
  }
  y[2167] += A[5427] * x[2166];
  y[2167] += A[5428] * x[2168];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2168] += A[1 * i0 + 5429] * x[1 * i0 + 2167];
  }
  y[2169] += A[5432] * x[2168];
  y[2169] += A[5433] * x[2170];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2170] += A[1 * i0 + 5434] * x[1 * i0 + 2169];
  }
  y[2171] += A[5437] * x[2170];
  y[2171] += A[5438] * x[2172];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2172] += A[1 * i0 + 5439] * x[1 * i0 + 2171];
  }
  y[2173] += A[5442] * x[2172];
  y[2173] += A[5443] * x[2174];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2174] += A[1 * i0 + 5444] * x[1 * i0 + 2173];
  }
  y[2175] += A[5447] * x[2174];
  y[2175] += A[5448] * x[2176];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2176] += A[1 * i0 + 5449] * x[1 * i0 + 2175];
  }
  y[2177] += A[5452] * x[2176];
  y[2177] += A[5453] * x[2178];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2178] += A[1 * i0 + 5454] * x[1 * i0 + 2177];
  }
  y[2179] += A[5457] * x[2178];
  y[2179] += A[5458] * x[2180];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2180] += A[1 * i0 + 5459] * x[1 * i0 + 2179];
  }
  y[2181] += A[5462] * x[2180];
  y[2181] += A[5463] * x[2182];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2182] += A[1 * i0 + 5464] * x[1 * i0 + 2181];
  }
  y[2183] += A[5467] * x[2182];
  y[2183] += A[5468] * x[2184];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2184] += A[1 * i0 + 5469] * x[1 * i0 + 2183];
  }
  y[2185] += A[5472] * x[2184];
  y[2185] += A[5473] * x[2186];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2186] += A[1 * i0 + 5474] * x[1 * i0 + 2185];
  }
  y[2187] += A[5477] * x[2186];
  y[2187] += A[5478] * x[2188];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2188] += A[1 * i0 + 5479] * x[1 * i0 + 2187];
  }
  y[2189] += A[5482] * x[2188];
  y[2189] += A[5483] * x[2190];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2190] += A[1 * i0 + 5484] * x[1 * i0 + 2189];
  }
  y[2191] += A[5487] * x[2190];
  y[2191] += A[5488] * x[2192];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2192] += A[1 * i0 + 5489] * x[1 * i0 + 2191];
  }
  y[2193] += A[5492] * x[2192];
  y[2193] += A[5493] * x[2194];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2194] += A[1 * i0 + 5494] * x[1 * i0 + 2193];
  }
  y[2195] += A[5497] * x[2194];
  y[2195] += A[5498] * x[2196];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2196] += A[1 * i0 + 5499] * x[1 * i0 + 2195];
  }
  y[2197] += A[5502] * x[2196];
  y[2197] += A[5503] * x[2198];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2198] += A[1 * i0 + 5504] * x[1 * i0 + 2197];
  }
  y[2199] += A[5507] * x[2198];
  y[2199] += A[5508] * x[2200];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2200] += A[1 * i0 + 5509] * x[1 * i0 + 2199];
  }
  y[2201] += A[5512] * x[2200];
  y[2201] += A[5513] * x[2202];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2202] += A[1 * i0 + 5514] * x[1 * i0 + 2201];
  }
  y[2203] += A[5517] * x[2202];
  y[2203] += A[5518] * x[2204];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2204] += A[1 * i0 + 5519] * x[1 * i0 + 2203];
  }
  y[2205] += A[5522] * x[2204];
  y[2205] += A[5523] * x[2206];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2206] += A[1 * i0 + 5524] * x[1 * i0 + 2205];
  }
  y[2207] += A[5527] * x[2206];
  y[2207] += A[5528] * x[2208];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2208] += A[1 * i0 + 5529] * x[1 * i0 + 2207];
  }
  y[2209] += A[5532] * x[2208];
  y[2209] += A[5533] * x[2210];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2210] += A[1 * i0 + 5534] * x[1 * i0 + 2209];
  }
  y[2211] += A[5537] * x[2210];
  y[2211] += A[5538] * x[2212];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2212] += A[1 * i0 + 5539] * x[1 * i0 + 2211];
  }
  y[2213] += A[5542] * x[2212];
  y[2213] += A[5543] * x[2214];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2214] += A[1 * i0 + 5544] * x[1 * i0 + 2213];
  }
  y[2215] += A[5547] * x[2214];
  y[2215] += A[5548] * x[2216];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2216] += A[1 * i0 + 5549] * x[1 * i0 + 2215];
  }
  y[2217] += A[5552] * x[2216];
  y[2217] += A[5553] * x[2218];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2218] += A[1 * i0 + 5554] * x[1 * i0 + 2217];
  }
  y[2219] += A[5557] * x[2218];
  y[2219] += A[5558] * x[2220];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2220] += A[1 * i0 + 5559] * x[1 * i0 + 2219];
  }
  y[2221] += A[5562] * x[2220];
  y[2221] += A[5563] * x[2222];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2222] += A[1 * i0 + 5564] * x[1 * i0 + 2221];
  }
  y[2223] += A[5567] * x[2222];
  y[2223] += A[5568] * x[2224];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2224] += A[1 * i0 + 5569] * x[1 * i0 + 2223];
  }
  y[2225] += A[5572] * x[2224];
  y[2225] += A[5573] * x[2226];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2226] += A[1 * i0 + 5574] * x[1 * i0 + 2225];
  }
  y[2227] += A[5577] * x[2226];
  y[2227] += A[5578] * x[2228];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2228] += A[1 * i0 + 5579] * x[1 * i0 + 2227];
  }
  y[2229] += A[5582] * x[2228];
  y[2229] += A[5583] * x[2230];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2230] += A[1 * i0 + 5584] * x[1 * i0 + 2229];
  }
  y[2231] += A[5587] * x[2230];
  y[2231] += A[5588] * x[2232];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2232] += A[1 * i0 + 5589] * x[1 * i0 + 2231];
  }
  y[2233] += A[5592] * x[2232];
  y[2233] += A[5593] * x[2234];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2234] += A[1 * i0 + 5594] * x[1 * i0 + 2233];
  }
  y[2235] += A[5597] * x[2234];
  y[2235] += A[5598] * x[2236];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2236] += A[1 * i0 + 5599] * x[1 * i0 + 2235];
  }
  y[2237] += A[5602] * x[2236];
  y[2237] += A[5603] * x[2238];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2238] += A[1 * i0 + 5604] * x[1 * i0 + 2237];
  }
  y[2239] += A[5607] * x[2238];
  y[2239] += A[5608] * x[2240];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2240] += A[1 * i0 + 5609] * x[1 * i0 + 2239];
  }
  y[2241] += A[5612] * x[2240];
  y[2241] += A[5613] * x[2242];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2242] += A[1 * i0 + 5614] * x[1 * i0 + 2241];
  }
  y[2243] += A[5617] * x[2242];
  y[2243] += A[5618] * x[2244];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2244] += A[1 * i0 + 5619] * x[1 * i0 + 2243];
  }
  y[2245] += A[5622] * x[2244];
  y[2245] += A[5623] * x[2246];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2246] += A[1 * i0 + 5624] * x[1 * i0 + 2245];
  }
  y[2247] += A[5627] * x[2246];
  y[2247] += A[5628] * x[2248];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2248] += A[1 * i0 + 5629] * x[1 * i0 + 2247];
  }
  y[2249] += A[5632] * x[2248];
  y[2249] += A[5633] * x[2250];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 2250] += A[1 * i0 + 5634] * x[1 * i0 + 2249];
  }
  y[2250] += A[5636] * x[2412];
  y[2251] += A[5637] * x[494];
  y[2251] += A[5638] * x[2092];
  y[2251] += A[5639] * x[2252];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2252] += A[1 * i0 + 5640] * x[1 * i0 + 2251];
  }
  y[2253] += A[5643] * x[2252];
  y[2253] += A[5644] * x[2254];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2254] += A[1 * i0 + 5645] * x[1 * i0 + 2253];
  }
  y[2255] += A[5648] * x[2254];
  y[2255] += A[5649] * x[2256];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2256] += A[1 * i0 + 5650] * x[1 * i0 + 2255];
  }
  y[2257] += A[5653] * x[2256];
  y[2257] += A[5654] * x[2258];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2258] += A[1 * i0 + 5655] * x[1 * i0 + 2257];
  }
  y[2259] += A[5658] * x[2258];
  y[2259] += A[5659] * x[2260];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2260] += A[1 * i0 + 5660] * x[1 * i0 + 2259];
  }
  y[2261] += A[5663] * x[2260];
  y[2261] += A[5664] * x[2262];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2262] += A[1 * i0 + 5665] * x[1 * i0 + 2261];
  }
  y[2263] += A[5668] * x[2262];
  y[2263] += A[5669] * x[2264];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2264] += A[1 * i0 + 5670] * x[1 * i0 + 2263];
  }
  y[2265] += A[5673] * x[2264];
  y[2265] += A[5674] * x[2266];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2266] += A[1 * i0 + 5675] * x[1 * i0 + 2265];
  }
  y[2267] += A[5678] * x[2266];
  y[2267] += A[5679] * x[2268];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2268] += A[1 * i0 + 5680] * x[1 * i0 + 2267];
  }
  y[2269] += A[5683] * x[2268];
  y[2269] += A[5684] * x[2270];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2270] += A[1 * i0 + 5685] * x[1 * i0 + 2269];
  }
  y[2271] += A[5688] * x[2270];
  y[2271] += A[5689] * x[2272];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2272] += A[1 * i0 + 5690] * x[1 * i0 + 2271];
  }
  y[2273] += A[5693] * x[2272];
  y[2273] += A[5694] * x[2274];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2274] += A[1 * i0 + 5695] * x[1 * i0 + 2273];
  }
  y[2275] += A[5698] * x[2274];
  y[2275] += A[5699] * x[2276];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2276] += A[1 * i0 + 5700] * x[1 * i0 + 2275];
  }
  y[2277] += A[5703] * x[2276];
  y[2277] += A[5704] * x[2278];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2278] += A[1 * i0 + 5705] * x[1 * i0 + 2277];
  }
  y[2279] += A[5708] * x[2278];
  y[2279] += A[5709] * x[2280];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2280] += A[1 * i0 + 5710] * x[1 * i0 + 2279];
  }
  y[2281] += A[5713] * x[2280];
  y[2281] += A[5714] * x[2282];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2282] += A[1 * i0 + 5715] * x[1 * i0 + 2281];
  }
  y[2283] += A[5718] * x[2282];
  y[2283] += A[5719] * x[2284];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2284] += A[1 * i0 + 5720] * x[1 * i0 + 2283];
  }
  y[2285] += A[5723] * x[2284];
  y[2285] += A[5724] * x[2286];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2286] += A[1 * i0 + 5725] * x[1 * i0 + 2285];
  }
  y[2287] += A[5728] * x[2286];
  y[2287] += A[5729] * x[2288];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2288] += A[1 * i0 + 5730] * x[1 * i0 + 2287];
  }
  y[2289] += A[5733] * x[2288];
  y[2289] += A[5734] * x[2290];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2290] += A[1 * i0 + 5735] * x[1 * i0 + 2289];
  }
  y[2291] += A[5738] * x[2290];
  y[2291] += A[5739] * x[2292];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2292] += A[1 * i0 + 5740] * x[1 * i0 + 2291];
  }
  y[2293] += A[5743] * x[2292];
  y[2293] += A[5744] * x[2294];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2294] += A[1 * i0 + 5745] * x[1 * i0 + 2293];
  }
  y[2295] += A[5748] * x[2294];
  y[2295] += A[5749] * x[2296];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2296] += A[1 * i0 + 5750] * x[1 * i0 + 2295];
  }
  y[2297] += A[5753] * x[2296];
  y[2297] += A[5754] * x[2298];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2298] += A[1 * i0 + 5755] * x[1 * i0 + 2297];
  }
  y[2299] += A[5758] * x[2298];
  y[2299] += A[5759] * x[2300];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2300] += A[1 * i0 + 5760] * x[1 * i0 + 2299];
  }
  y[2301] += A[5763] * x[2300];
  y[2301] += A[5764] * x[2302];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2302] += A[1 * i0 + 5765] * x[1 * i0 + 2301];
  }
  y[2303] += A[5768] * x[2302];
  y[2303] += A[5769] * x[2304];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2304] += A[1 * i0 + 5770] * x[1 * i0 + 2303];
  }
  y[2305] += A[5773] * x[2304];
  y[2305] += A[5774] * x[2306];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2306] += A[1 * i0 + 5775] * x[1 * i0 + 2305];
  }
  y[2307] += A[5778] * x[2306];
  y[2307] += A[5779] * x[2308];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2308] += A[1 * i0 + 5780] * x[1 * i0 + 2307];
  }
  y[2309] += A[5783] * x[2308];
  y[2309] += A[5784] * x[2310];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2310] += A[1 * i0 + 5785] * x[1 * i0 + 2309];
  }
  y[2311] += A[5788] * x[2310];
  y[2311] += A[5789] * x[2312];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2312] += A[1 * i0 + 5790] * x[1 * i0 + 2311];
  }
  y[2313] += A[5793] * x[2312];
  y[2313] += A[5794] * x[2314];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2314] += A[1 * i0 + 5795] * x[1 * i0 + 2313];
  }
  y[2315] += A[5798] * x[2314];
  y[2315] += A[5799] * x[2316];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2316] += A[1 * i0 + 5800] * x[1 * i0 + 2315];
  }
  y[2317] += A[5803] * x[2316];
  y[2317] += A[5804] * x[2318];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2318] += A[1 * i0 + 5805] * x[1 * i0 + 2317];
  }
  y[2319] += A[5808] * x[2318];
  y[2319] += A[5809] * x[2320];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2320] += A[1 * i0 + 5810] * x[1 * i0 + 2319];
  }
  y[2321] += A[5813] * x[2320];
  y[2321] += A[5814] * x[2322];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2322] += A[1 * i0 + 5815] * x[1 * i0 + 2321];
  }
  y[2323] += A[5818] * x[2322];
  y[2323] += A[5819] * x[2324];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2324] += A[1 * i0 + 5820] * x[1 * i0 + 2323];
  }
  y[2325] += A[5823] * x[2324];
  y[2325] += A[5824] * x[2326];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2326] += A[1 * i0 + 5825] * x[1 * i0 + 2325];
  }
  y[2327] += A[5828] * x[2326];
  y[2327] += A[5829] * x[2328];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2328] += A[1 * i0 + 5830] * x[1 * i0 + 2327];
  }
  y[2329] += A[5833] * x[2328];
  y[2329] += A[5834] * x[2330];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2330] += A[1 * i0 + 5835] * x[1 * i0 + 2329];
  }
  y[2331] += A[5838] * x[2330];
  y[2331] += A[5839] * x[2332];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2332] += A[1 * i0 + 5840] * x[1 * i0 + 2331];
  }
  y[2333] += A[5843] * x[2332];
  y[2333] += A[5844] * x[2334];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2334] += A[1 * i0 + 5845] * x[1 * i0 + 2333];
  }
  y[2335] += A[5848] * x[2334];
  y[2335] += A[5849] * x[2336];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2336] += A[1 * i0 + 5850] * x[1 * i0 + 2335];
  }
  y[2337] += A[5853] * x[2336];
  y[2337] += A[5854] * x[2338];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2338] += A[1 * i0 + 5855] * x[1 * i0 + 2337];
  }
  y[2339] += A[5858] * x[2338];
  y[2339] += A[5859] * x[2340];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2340] += A[1 * i0 + 5860] * x[1 * i0 + 2339];
  }
  y[2341] += A[5863] * x[2340];
  y[2341] += A[5864] * x[2342];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2342] += A[1 * i0 + 5865] * x[1 * i0 + 2341];
  }
  y[2343] += A[5868] * x[2342];
  y[2343] += A[5869] * x[2344];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2344] += A[1 * i0 + 5870] * x[1 * i0 + 2343];
  }
  y[2345] += A[5873] * x[2344];
  y[2345] += A[5874] * x[2346];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2346] += A[1 * i0 + 5875] * x[1 * i0 + 2345];
  }
  y[2347] += A[5878] * x[2346];
  y[2347] += A[5879] * x[2348];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2348] += A[1 * i0 + 5880] * x[1 * i0 + 2347];
  }
  y[2349] += A[5883] * x[2348];
  y[2349] += A[5884] * x[2350];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2350] += A[1 * i0 + 5885] * x[1 * i0 + 2349];
  }
  y[2351] += A[5888] * x[2350];
  y[2351] += A[5889] * x[2352];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2352] += A[1 * i0 + 5890] * x[1 * i0 + 2351];
  }
  y[2353] += A[5893] * x[2352];
  y[2353] += A[5894] * x[2354];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2354] += A[1 * i0 + 5895] * x[1 * i0 + 2353];
  }
  y[2355] += A[5898] * x[2354];
  y[2355] += A[5899] * x[2356];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2356] += A[1 * i0 + 5900] * x[1 * i0 + 2355];
  }
  y[2357] += A[5903] * x[2356];
  y[2357] += A[5904] * x[2358];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2358] += A[1 * i0 + 5905] * x[1 * i0 + 2357];
  }
  y[2359] += A[5908] * x[2358];
  y[2359] += A[5909] * x[2360];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2360] += A[1 * i0 + 5910] * x[1 * i0 + 2359];
  }
  y[2361] += A[5913] * x[2360];
  y[2361] += A[5914] * x[2362];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2362] += A[1 * i0 + 5915] * x[1 * i0 + 2361];
  }
  y[2363] += A[5918] * x[2362];
  y[2363] += A[5919] * x[2364];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2364] += A[1 * i0 + 5920] * x[1 * i0 + 2363];
  }
  y[2365] += A[5923] * x[2364];
  y[2365] += A[5924] * x[2366];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2366] += A[1 * i0 + 5925] * x[1 * i0 + 2365];
  }
  y[2367] += A[5928] * x[2366];
  y[2367] += A[5929] * x[2368];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2368] += A[1 * i0 + 5930] * x[1 * i0 + 2367];
  }
  y[2369] += A[5933] * x[2368];
  y[2369] += A[5934] * x[2370];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2370] += A[1 * i0 + 5935] * x[1 * i0 + 2369];
  }
  y[2371] += A[5938] * x[2370];
  y[2371] += A[5939] * x[2372];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2372] += A[1 * i0 + 5940] * x[1 * i0 + 2371];
  }
  y[2373] += A[5943] * x[2372];
  y[2373] += A[5944] * x[2374];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2374] += A[1 * i0 + 5945] * x[1 * i0 + 2373];
  }
  y[2375] += A[5948] * x[2374];
  y[2375] += A[5949] * x[2376];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2376] += A[1 * i0 + 5950] * x[1 * i0 + 2375];
  }
  y[2377] += A[5953] * x[2376];
  y[2377] += A[5954] * x[2378];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2378] += A[1 * i0 + 5955] * x[1 * i0 + 2377];
  }
  y[2379] += A[5958] * x[2378];
  y[2379] += A[5959] * x[2380];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2380] += A[1 * i0 + 5960] * x[1 * i0 + 2379];
  }
  y[2381] += A[5963] * x[2380];
  y[2381] += A[5964] * x[2382];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2382] += A[1 * i0 + 5965] * x[1 * i0 + 2381];
  }
  y[2383] += A[5968] * x[2382];
  y[2383] += A[5969] * x[2384];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2384] += A[1 * i0 + 5970] * x[1 * i0 + 2383];
  }
  y[2385] += A[5973] * x[2384];
  y[2385] += A[5974] * x[2386];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2386] += A[1 * i0 + 5975] * x[1 * i0 + 2385];
  }
  y[2387] += A[5978] * x[2386];
  y[2387] += A[5979] * x[2388];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2388] += A[1 * i0 + 5980] * x[1 * i0 + 2387];
  }
  y[2389] += A[5983] * x[2388];
  y[2389] += A[5984] * x[2390];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2390] += A[1 * i0 + 5985] * x[1 * i0 + 2389];
  }
  y[2391] += A[5988] * x[2390];
  y[2391] += A[5989] * x[2392];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2392] += A[1 * i0 + 5990] * x[1 * i0 + 2391];
  }
  y[2393] += A[5993] * x[2392];
  y[2393] += A[5994] * x[2394];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2394] += A[1 * i0 + 5995] * x[1 * i0 + 2393];
  }
  y[2395] += A[5998] * x[2394];
  y[2395] += A[5999] * x[2396];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2396] += A[1 * i0 + 6000] * x[1 * i0 + 2395];
  }
  y[2397] += A[6003] * x[2396];
  y[2397] += A[6004] * x[2398];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2398] += A[1 * i0 + 6005] * x[1 * i0 + 2397];
  }
  y[2399] += A[6008] * x[2398];
  y[2399] += A[6009] * x[2400];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2400] += A[1 * i0 + 6010] * x[1 * i0 + 2399];
  }
  y[2401] += A[6013] * x[2400];
  y[2401] += A[6014] * x[2402];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2402] += A[1 * i0 + 6015] * x[1 * i0 + 2401];
  }
  y[2403] += A[6018] * x[2402];
  y[2403] += A[6019] * x[2404];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2404] += A[1 * i0 + 6020] * x[1 * i0 + 2403];
  }
  y[2405] += A[6023] * x[2404];
  y[2405] += A[6024] * x[2406];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2406] += A[1 * i0 + 6025] * x[1 * i0 + 2405];
  }
  y[2407] += A[6028] * x[2406];
  y[2407] += A[6029] * x[2408];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2408] += A[1 * i0 + 6030] * x[1 * i0 + 2407];
  }
  y[2409] += A[6033] * x[2408];
  y[2409] += A[6034] * x[2410];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2410] += A[1 * i0 + 6035] * x[1 * i0 + 2409];
  }
  y[2411] += A[6038] * x[2410];
  y[2411] += A[6039] * x[4019];
  y[2412] += A[6040] * x[8];
  y[2412] += A[6041] * x[2250];
  y[2412] += A[6042] * x[2413];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2413] += A[1 * i0 + 6043] * x[1 * i0 + 2412];
  }
  y[2414] += A[6046] * x[2413];
  y[2414] += A[6047] * x[2415];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2415] += A[1 * i0 + 6048] * x[1 * i0 + 2414];
  }
  y[2416] += A[6051] * x[2415];
  y[2416] += A[6052] * x[2417];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2417] += A[1 * i0 + 6053] * x[1 * i0 + 2416];
  }
  y[2418] += A[6056] * x[2417];
  y[2418] += A[6057] * x[2419];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2419] += A[1 * i0 + 6058] * x[1 * i0 + 2418];
  }
  y[2420] += A[6061] * x[2419];
  y[2420] += A[6062] * x[2421];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2421] += A[1 * i0 + 6063] * x[1 * i0 + 2420];
  }
  y[2422] += A[6066] * x[2421];
  y[2422] += A[6067] * x[2423];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2423] += A[1 * i0 + 6068] * x[1 * i0 + 2422];
  }
  y[2424] += A[6071] * x[2423];
  y[2424] += A[6072] * x[2425];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2425] += A[1 * i0 + 6073] * x[1 * i0 + 2424];
  }
  y[2426] += A[6076] * x[2425];
  y[2426] += A[6077] * x[2427];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2427] += A[1 * i0 + 6078] * x[1 * i0 + 2426];
  }
  y[2428] += A[6081] * x[2427];
  y[2428] += A[6082] * x[2429];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2429] += A[1 * i0 + 6083] * x[1 * i0 + 2428];
  }
  y[2430] += A[6086] * x[2429];
  y[2430] += A[6087] * x[2431];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2431] += A[1 * i0 + 6088] * x[1 * i0 + 2430];
  }
  y[2432] += A[6091] * x[2431];
  y[2432] += A[6092] * x[2433];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2433] += A[1 * i0 + 6093] * x[1 * i0 + 2432];
  }
  y[2434] += A[6096] * x[2433];
  y[2434] += A[6097] * x[2435];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2435] += A[1 * i0 + 6098] * x[1 * i0 + 2434];
  }
  y[2436] += A[6101] * x[2435];
  y[2436] += A[6102] * x[2437];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2437] += A[1 * i0 + 6103] * x[1 * i0 + 2436];
  }
  y[2438] += A[6106] * x[2437];
  y[2438] += A[6107] * x[2439];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2439] += A[1 * i0 + 6108] * x[1 * i0 + 2438];
  }
  y[2440] += A[6111] * x[2439];
  y[2440] += A[6112] * x[2441];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2441] += A[1 * i0 + 6113] * x[1 * i0 + 2440];
  }
  y[2442] += A[6116] * x[2441];
  y[2442] += A[6117] * x[2443];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2443] += A[1 * i0 + 6118] * x[1 * i0 + 2442];
  }
  y[2444] += A[6121] * x[2443];
  y[2444] += A[6122] * x[2445];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2445] += A[1 * i0 + 6123] * x[1 * i0 + 2444];
  }
  y[2446] += A[6126] * x[2445];
  y[2446] += A[6127] * x[2447];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2447] += A[1 * i0 + 6128] * x[1 * i0 + 2446];
  }
  y[2448] += A[6131] * x[2447];
  y[2448] += A[6132] * x[2449];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2449] += A[1 * i0 + 6133] * x[1 * i0 + 2448];
  }
  y[2450] += A[6136] * x[2449];
  y[2450] += A[6137] * x[2451];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2451] += A[1 * i0 + 6138] * x[1 * i0 + 2450];
  }
  y[2452] += A[6141] * x[2451];
  y[2452] += A[6142] * x[2453];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2453] += A[1 * i0 + 6143] * x[1 * i0 + 2452];
  }
  y[2454] += A[6146] * x[2453];
  y[2454] += A[6147] * x[2455];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2455] += A[1 * i0 + 6148] * x[1 * i0 + 2454];
  }
  y[2456] += A[6151] * x[2455];
  y[2456] += A[6152] * x[2457];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2457] += A[1 * i0 + 6153] * x[1 * i0 + 2456];
  }
  y[2458] += A[6156] * x[2457];
  y[2458] += A[6157] * x[2459];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2459] += A[1 * i0 + 6158] * x[1 * i0 + 2458];
  }
  y[2460] += A[6161] * x[2459];
  y[2460] += A[6162] * x[2461];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2461] += A[1 * i0 + 6163] * x[1 * i0 + 2460];
  }
  y[2462] += A[6166] * x[2461];
  y[2462] += A[6167] * x[2463];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2463] += A[1 * i0 + 6168] * x[1 * i0 + 2462];
  }
  y[2464] += A[6171] * x[2463];
  y[2464] += A[6172] * x[2465];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2465] += A[1 * i0 + 6173] * x[1 * i0 + 2464];
  }
  y[2466] += A[6176] * x[2465];
  y[2466] += A[6177] * x[2467];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2467] += A[1 * i0 + 6178] * x[1 * i0 + 2466];
  }
  y[2468] += A[6181] * x[2467];
  y[2468] += A[6182] * x[2469];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2469] += A[1 * i0 + 6183] * x[1 * i0 + 2468];
  }
  y[2470] += A[6186] * x[2469];
  y[2470] += A[6187] * x[2471];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2471] += A[1 * i0 + 6188] * x[1 * i0 + 2470];
  }
  y[2472] += A[6191] * x[2471];
  y[2472] += A[6192] * x[2473];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2473] += A[1 * i0 + 6193] * x[1 * i0 + 2472];
  }
  y[2474] += A[6196] * x[2473];
  y[2474] += A[6197] * x[2475];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2475] += A[1 * i0 + 6198] * x[1 * i0 + 2474];
  }
  y[2476] += A[6201] * x[2475];
  y[2476] += A[6202] * x[2477];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2477] += A[1 * i0 + 6203] * x[1 * i0 + 2476];
  }
  y[2478] += A[6206] * x[2477];
  y[2478] += A[6207] * x[2479];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2479] += A[1 * i0 + 6208] * x[1 * i0 + 2478];
  }
  y[2480] += A[6211] * x[2479];
  y[2480] += A[6212] * x[2481];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2481] += A[1 * i0 + 6213] * x[1 * i0 + 2480];
  }
  y[2482] += A[6216] * x[2481];
  y[2482] += A[6217] * x[2483];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2483] += A[1 * i0 + 6218] * x[1 * i0 + 2482];
  }
  y[2484] += A[6221] * x[2483];
  y[2484] += A[6222] * x[2485];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2485] += A[1 * i0 + 6223] * x[1 * i0 + 2484];
  }
  y[2486] += A[6226] * x[2485];
  y[2486] += A[6227] * x[2487];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2487] += A[1 * i0 + 6228] * x[1 * i0 + 2486];
  }
  y[2488] += A[6231] * x[2487];
  y[2488] += A[6232] * x[2489];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2489] += A[1 * i0 + 6233] * x[1 * i0 + 2488];
  }
  y[2490] += A[6236] * x[2489];
  y[2490] += A[6237] * x[2491];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2491] += A[1 * i0 + 6238] * x[1 * i0 + 2490];
  }
  y[2492] += A[6241] * x[2491];
  y[2492] += A[6242] * x[2493];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2493] += A[1 * i0 + 6243] * x[1 * i0 + 2492];
  }
  y[2494] += A[6246] * x[2493];
  y[2494] += A[6247] * x[2495];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2495] += A[1 * i0 + 6248] * x[1 * i0 + 2494];
  }
  y[2496] += A[6251] * x[2495];
  y[2496] += A[6252] * x[2497];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2497] += A[1 * i0 + 6253] * x[1 * i0 + 2496];
  }
  y[2498] += A[6256] * x[2497];
  y[2498] += A[6257] * x[2499];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2499] += A[1 * i0 + 6258] * x[1 * i0 + 2498];
  }
  y[2500] += A[6261] * x[2499];
  y[2500] += A[6262] * x[2501];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2501] += A[1 * i0 + 6263] * x[1 * i0 + 2500];
  }
  y[2502] += A[6266] * x[2501];
  y[2502] += A[6267] * x[2503];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2503] += A[1 * i0 + 6268] * x[1 * i0 + 2502];
  }
  y[2504] += A[6271] * x[2503];
  y[2504] += A[6272] * x[2505];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2505] += A[1 * i0 + 6273] * x[1 * i0 + 2504];
  }
  y[2506] += A[6276] * x[2505];
  y[2506] += A[6277] * x[2507];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2507] += A[1 * i0 + 6278] * x[1 * i0 + 2506];
  }
  y[2508] += A[6281] * x[2507];
  y[2508] += A[6282] * x[2509];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2509] += A[1 * i0 + 6283] * x[1 * i0 + 2508];
  }
  y[2510] += A[6286] * x[2509];
  y[2510] += A[6287] * x[2511];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2511] += A[1 * i0 + 6288] * x[1 * i0 + 2510];
  }
  y[2512] += A[6291] * x[2511];
  y[2512] += A[6292] * x[2513];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2513] += A[1 * i0 + 6293] * x[1 * i0 + 2512];
  }
  y[2514] += A[6296] * x[2513];
  y[2514] += A[6297] * x[2515];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2515] += A[1 * i0 + 6298] * x[1 * i0 + 2514];
  }
  y[2516] += A[6301] * x[2515];
  y[2516] += A[6302] * x[2517];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2517] += A[1 * i0 + 6303] * x[1 * i0 + 2516];
  }
  y[2518] += A[6306] * x[2517];
  y[2518] += A[6307] * x[2519];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2519] += A[1 * i0 + 6308] * x[1 * i0 + 2518];
  }
  y[2520] += A[6311] * x[2519];
  y[2520] += A[6312] * x[2521];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2521] += A[1 * i0 + 6313] * x[1 * i0 + 2520];
  }
  y[2522] += A[6316] * x[2521];
  y[2522] += A[6317] * x[2523];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2523] += A[1 * i0 + 6318] * x[1 * i0 + 2522];
  }
  y[2524] += A[6321] * x[2523];
  y[2524] += A[6322] * x[2525];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2525] += A[1 * i0 + 6323] * x[1 * i0 + 2524];
  }
  y[2526] += A[6326] * x[2525];
  y[2526] += A[6327] * x[2527];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2527] += A[1 * i0 + 6328] * x[1 * i0 + 2526];
  }
}

void kernel_spmv_fragment_5(double *restrict A, double *restrict x,
                            double *restrict y) {
  register int i0, i1, i2, i3, i4, i5, i6;

  y[2528] += A[6331] * x[2527];
  y[2528] += A[6332] * x[2529];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2529] += A[1 * i0 + 6333] * x[1 * i0 + 2528];
  }
  y[2530] += A[6336] * x[2529];
  y[2530] += A[6337] * x[2531];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2531] += A[1 * i0 + 6338] * x[1 * i0 + 2530];
  }
  y[2532] += A[6341] * x[2531];
  y[2532] += A[6342] * x[2533];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2533] += A[1 * i0 + 6343] * x[1 * i0 + 2532];
  }
  y[2534] += A[6346] * x[2533];
  y[2534] += A[6347] * x[2535];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2535] += A[1 * i0 + 6348] * x[1 * i0 + 2534];
  }
  y[2536] += A[6351] * x[2535];
  y[2536] += A[6352] * x[2537];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2537] += A[1 * i0 + 6353] * x[1 * i0 + 2536];
  }
  y[2538] += A[6356] * x[2537];
  y[2538] += A[6357] * x[2539];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2539] += A[1 * i0 + 6358] * x[1 * i0 + 2538];
  }
  y[2540] += A[6361] * x[2539];
  y[2540] += A[6362] * x[2541];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2541] += A[1 * i0 + 6363] * x[1 * i0 + 2540];
  }
  y[2542] += A[6366] * x[2541];
  y[2542] += A[6367] * x[2543];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2543] += A[1 * i0 + 6368] * x[1 * i0 + 2542];
  }
  y[2544] += A[6371] * x[2543];
  y[2544] += A[6372] * x[2545];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2545] += A[1 * i0 + 6373] * x[1 * i0 + 2544];
  }
  y[2546] += A[6376] * x[2545];
  y[2546] += A[6377] * x[2547];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2547] += A[1 * i0 + 6378] * x[1 * i0 + 2546];
  }
  y[2548] += A[6381] * x[2547];
  y[2548] += A[6382] * x[2549];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2549] += A[1 * i0 + 6383] * x[1 * i0 + 2548];
  }
  y[2550] += A[6386] * x[2549];
  y[2550] += A[6387] * x[2551];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2551] += A[1 * i0 + 6388] * x[1 * i0 + 2550];
  }
  y[2552] += A[6391] * x[2551];
  y[2552] += A[6392] * x[2553];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2553] += A[1 * i0 + 6393] * x[1 * i0 + 2552];
  }
  y[2554] += A[6396] * x[2553];
  y[2554] += A[6397] * x[2555];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2555] += A[1 * i0 + 6398] * x[1 * i0 + 2554];
  }
  y[2556] += A[6401] * x[2555];
  y[2556] += A[6402] * x[2557];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2557] += A[1 * i0 + 6403] * x[1 * i0 + 2556];
  }
  y[2558] += A[6406] * x[2557];
  y[2558] += A[6407] * x[2559];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2559] += A[1 * i0 + 6408] * x[1 * i0 + 2558];
  }
  y[2560] += A[6411] * x[2559];
  y[2560] += A[6412] * x[2561];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2561] += A[1 * i0 + 6413] * x[1 * i0 + 2560];
  }
  y[2562] += A[6416] * x[2561];
  y[2562] += A[6417] * x[2563];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2563] += A[1 * i0 + 6418] * x[1 * i0 + 2562];
  }
  y[2564] += A[6421] * x[2563];
  y[2564] += A[6422] * x[2565];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2565] += A[1 * i0 + 6423] * x[1 * i0 + 2564];
  }
  y[2566] += A[6426] * x[2565];
  y[2566] += A[6427] * x[2567];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2567] += A[1 * i0 + 6428] * x[1 * i0 + 2566];
  }
  y[2568] += A[6431] * x[2567];
  y[2568] += A[6432] * x[2569];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2569] += A[1 * i0 + 6433] * x[1 * i0 + 2568];
  }
  y[2570] += A[6436] * x[2569];
  y[2570] += A[6437] * x[2571];
  y[2571] += A[6438] * x[2570];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[1 * i0 + 2571] += A[1 * i0 + 6439] * x[1 * i0 + 2571];
  }
  y[2572] += A[6441] * x[2573];
  y[2572] += A[6442] * x[2732];
  y[2573] += A[6443] * x[2572];
  y[2573] += A[6444] * x[2574];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2574] += A[1 * i0 + 6445] * x[1 * i0 + 2573];
  }
  y[2575] += A[6448] * x[2574];
  y[2575] += A[6449] * x[2576];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2576] += A[1 * i0 + 6450] * x[1 * i0 + 2575];
  }
  y[2577] += A[6453] * x[2576];
  y[2577] += A[6454] * x[2578];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2578] += A[1 * i0 + 6455] * x[1 * i0 + 2577];
  }
  y[2579] += A[6458] * x[2578];
  y[2579] += A[6459] * x[2580];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2580] += A[1 * i0 + 6460] * x[1 * i0 + 2579];
  }
  y[2581] += A[6463] * x[2580];
  y[2581] += A[6464] * x[2582];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2582] += A[1 * i0 + 6465] * x[1 * i0 + 2581];
  }
  y[2583] += A[6468] * x[2582];
  y[2583] += A[6469] * x[2584];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2584] += A[1 * i0 + 6470] * x[1 * i0 + 2583];
  }
  y[2585] += A[6473] * x[2584];
  y[2585] += A[6474] * x[2586];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2586] += A[1 * i0 + 6475] * x[1 * i0 + 2585];
  }
  y[2587] += A[6478] * x[2586];
  y[2587] += A[6479] * x[2588];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2588] += A[1 * i0 + 6480] * x[1 * i0 + 2587];
  }
  y[2589] += A[6483] * x[2588];
  y[2589] += A[6484] * x[2590];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2590] += A[1 * i0 + 6485] * x[1 * i0 + 2589];
  }
  y[2591] += A[6488] * x[2590];
  y[2591] += A[6489] * x[2592];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2592] += A[1 * i0 + 6490] * x[1 * i0 + 2591];
  }
  y[2593] += A[6493] * x[2592];
  y[2593] += A[6494] * x[2594];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2594] += A[1 * i0 + 6495] * x[1 * i0 + 2593];
  }
  y[2595] += A[6498] * x[2594];
  y[2595] += A[6499] * x[2596];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2596] += A[1 * i0 + 6500] * x[1 * i0 + 2595];
  }
  y[2597] += A[6503] * x[2596];
  y[2597] += A[6504] * x[2598];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2598] += A[1 * i0 + 6505] * x[1 * i0 + 2597];
  }
  y[2599] += A[6508] * x[2598];
  y[2599] += A[6509] * x[2600];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2600] += A[1 * i0 + 6510] * x[1 * i0 + 2599];
  }
  y[2601] += A[6513] * x[2600];
  y[2601] += A[6514] * x[2602];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2602] += A[1 * i0 + 6515] * x[1 * i0 + 2601];
  }
  y[2603] += A[6518] * x[2602];
  y[2603] += A[6519] * x[2604];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2604] += A[1 * i0 + 6520] * x[1 * i0 + 2603];
  }
  y[2605] += A[6523] * x[2604];
  y[2605] += A[6524] * x[2606];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2606] += A[1 * i0 + 6525] * x[1 * i0 + 2605];
  }
  y[2607] += A[6528] * x[2606];
  y[2607] += A[6529] * x[2608];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2608] += A[1 * i0 + 6530] * x[1 * i0 + 2607];
  }
  y[2609] += A[6533] * x[2608];
  y[2609] += A[6534] * x[2610];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2610] += A[1 * i0 + 6535] * x[1 * i0 + 2609];
  }
  y[2611] += A[6538] * x[2610];
  y[2611] += A[6539] * x[2612];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2612] += A[1 * i0 + 6540] * x[1 * i0 + 2611];
  }
  y[2613] += A[6543] * x[2612];
  y[2613] += A[6544] * x[2614];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2614] += A[1 * i0 + 6545] * x[1 * i0 + 2613];
  }
  y[2615] += A[6548] * x[2614];
  y[2615] += A[6549] * x[2616];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2616] += A[1 * i0 + 6550] * x[1 * i0 + 2615];
  }
  y[2617] += A[6553] * x[2616];
  y[2617] += A[6554] * x[2618];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2618] += A[1 * i0 + 6555] * x[1 * i0 + 2617];
  }
  y[2619] += A[6558] * x[2618];
  y[2619] += A[6559] * x[2620];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2620] += A[1 * i0 + 6560] * x[1 * i0 + 2619];
  }
  y[2621] += A[6563] * x[2620];
  y[2621] += A[6564] * x[2622];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2622] += A[1 * i0 + 6565] * x[1 * i0 + 2621];
  }
  y[2623] += A[6568] * x[2622];
  y[2623] += A[6569] * x[2624];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2624] += A[1 * i0 + 6570] * x[1 * i0 + 2623];
  }
  y[2625] += A[6573] * x[2624];
  y[2625] += A[6574] * x[2626];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2626] += A[1 * i0 + 6575] * x[1 * i0 + 2625];
  }
  y[2627] += A[6578] * x[2626];
  y[2627] += A[6579] * x[2628];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2628] += A[1 * i0 + 6580] * x[1 * i0 + 2627];
  }
  y[2629] += A[6583] * x[2628];
  y[2629] += A[6584] * x[2630];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2630] += A[1 * i0 + 6585] * x[1 * i0 + 2629];
  }
  y[2631] += A[6588] * x[2630];
  y[2631] += A[6589] * x[2632];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2632] += A[1 * i0 + 6590] * x[1 * i0 + 2631];
  }
  y[2633] += A[6593] * x[2632];
  y[2633] += A[6594] * x[2634];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2634] += A[1 * i0 + 6595] * x[1 * i0 + 2633];
  }
  y[2635] += A[6598] * x[2634];
  y[2635] += A[6599] * x[2636];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2636] += A[1 * i0 + 6600] * x[1 * i0 + 2635];
  }
  y[2637] += A[6603] * x[2636];
  y[2637] += A[6604] * x[2638];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2638] += A[1 * i0 + 6605] * x[1 * i0 + 2637];
  }
  y[2639] += A[6608] * x[2638];
  y[2639] += A[6609] * x[2640];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2640] += A[1 * i0 + 6610] * x[1 * i0 + 2639];
  }
  y[2641] += A[6613] * x[2640];
  y[2641] += A[6614] * x[2642];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2642] += A[1 * i0 + 6615] * x[1 * i0 + 2641];
  }
  y[2643] += A[6618] * x[2642];
  y[2643] += A[6619] * x[2644];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2644] += A[1 * i0 + 6620] * x[1 * i0 + 2643];
  }
  y[2645] += A[6623] * x[2644];
  y[2645] += A[6624] * x[2646];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2646] += A[1 * i0 + 6625] * x[1 * i0 + 2645];
  }
  y[2647] += A[6628] * x[2646];
  y[2647] += A[6629] * x[2648];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2648] += A[1 * i0 + 6630] * x[1 * i0 + 2647];
  }
  y[2649] += A[6633] * x[2648];
  y[2649] += A[6634] * x[2650];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2650] += A[1 * i0 + 6635] * x[1 * i0 + 2649];
  }
  y[2651] += A[6638] * x[2650];
  y[2651] += A[6639] * x[2652];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2652] += A[1 * i0 + 6640] * x[1 * i0 + 2651];
  }
  y[2653] += A[6643] * x[2652];
  y[2653] += A[6644] * x[2654];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2654] += A[1 * i0 + 6645] * x[1 * i0 + 2653];
  }
  y[2655] += A[6648] * x[2654];
  y[2655] += A[6649] * x[2656];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2656] += A[1 * i0 + 6650] * x[1 * i0 + 2655];
  }
  y[2657] += A[6653] * x[2656];
  y[2657] += A[6654] * x[2658];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2658] += A[1 * i0 + 6655] * x[1 * i0 + 2657];
  }
  y[2659] += A[6658] * x[2658];
  y[2659] += A[6659] * x[2660];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2660] += A[1 * i0 + 6660] * x[1 * i0 + 2659];
  }
  y[2661] += A[6663] * x[2660];
  y[2661] += A[6664] * x[2662];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2662] += A[1 * i0 + 6665] * x[1 * i0 + 2661];
  }
  y[2663] += A[6668] * x[2662];
  y[2663] += A[6669] * x[2664];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2664] += A[1 * i0 + 6670] * x[1 * i0 + 2663];
  }
  y[2665] += A[6673] * x[2664];
  y[2665] += A[6674] * x[2666];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2666] += A[1 * i0 + 6675] * x[1 * i0 + 2665];
  }
  y[2667] += A[6678] * x[2666];
  y[2667] += A[6679] * x[2668];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2668] += A[1 * i0 + 6680] * x[1 * i0 + 2667];
  }
  y[2669] += A[6683] * x[2668];
  y[2669] += A[6684] * x[2670];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2670] += A[1 * i0 + 6685] * x[1 * i0 + 2669];
  }
  y[2671] += A[6688] * x[2670];
  y[2671] += A[6689] * x[2672];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2672] += A[1 * i0 + 6690] * x[1 * i0 + 2671];
  }
  y[2673] += A[6693] * x[2672];
  y[2673] += A[6694] * x[2674];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2674] += A[1 * i0 + 6695] * x[1 * i0 + 2673];
  }
  y[2675] += A[6698] * x[2674];
  y[2675] += A[6699] * x[2676];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2676] += A[1 * i0 + 6700] * x[1 * i0 + 2675];
  }
  y[2677] += A[6703] * x[2676];
  y[2677] += A[6704] * x[2678];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2678] += A[1 * i0 + 6705] * x[1 * i0 + 2677];
  }
  y[2679] += A[6708] * x[2678];
  y[2679] += A[6709] * x[2680];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2680] += A[1 * i0 + 6710] * x[1 * i0 + 2679];
  }
  y[2681] += A[6713] * x[2680];
  y[2681] += A[6714] * x[2682];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2682] += A[1 * i0 + 6715] * x[1 * i0 + 2681];
  }
  y[2683] += A[6718] * x[2682];
  y[2683] += A[6719] * x[2684];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2684] += A[1 * i0 + 6720] * x[1 * i0 + 2683];
  }
  y[2685] += A[6723] * x[2684];
  y[2685] += A[6724] * x[2686];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2686] += A[1 * i0 + 6725] * x[1 * i0 + 2685];
  }
  y[2687] += A[6728] * x[2686];
  y[2687] += A[6729] * x[2688];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2688] += A[1 * i0 + 6730] * x[1 * i0 + 2687];
  }
  y[2689] += A[6733] * x[2688];
  y[2689] += A[6734] * x[2690];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2690] += A[1 * i0 + 6735] * x[1 * i0 + 2689];
  }
  y[2691] += A[6738] * x[2690];
  y[2691] += A[6739] * x[2692];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2692] += A[1 * i0 + 6740] * x[1 * i0 + 2691];
  }
  y[2693] += A[6743] * x[2692];
  y[2693] += A[6744] * x[2694];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2694] += A[1 * i0 + 6745] * x[1 * i0 + 2693];
  }
  y[2695] += A[6748] * x[2694];
  y[2695] += A[6749] * x[2696];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2696] += A[1 * i0 + 6750] * x[1 * i0 + 2695];
  }
  y[2697] += A[6753] * x[2696];
  y[2697] += A[6754] * x[2698];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2698] += A[1 * i0 + 6755] * x[1 * i0 + 2697];
  }
  y[2699] += A[6758] * x[2698];
  y[2699] += A[6759] * x[2700];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2700] += A[1 * i0 + 6760] * x[1 * i0 + 2699];
  }
  y[2701] += A[6763] * x[2700];
  y[2701] += A[6764] * x[2702];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2702] += A[1 * i0 + 6765] * x[1 * i0 + 2701];
  }
  y[2703] += A[6768] * x[2702];
  y[2703] += A[6769] * x[2704];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2704] += A[1 * i0 + 6770] * x[1 * i0 + 2703];
  }
  y[2705] += A[6773] * x[2704];
  y[2705] += A[6774] * x[2706];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2706] += A[1 * i0 + 6775] * x[1 * i0 + 2705];
  }
  y[2707] += A[6778] * x[2706];
  y[2707] += A[6779] * x[2708];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2708] += A[1 * i0 + 6780] * x[1 * i0 + 2707];
  }
  y[2709] += A[6783] * x[2708];
  y[2709] += A[6784] * x[2710];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2710] += A[1 * i0 + 6785] * x[1 * i0 + 2709];
  }
  y[2711] += A[6788] * x[2710];
  y[2711] += A[6789] * x[2712];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2712] += A[1 * i0 + 6790] * x[1 * i0 + 2711];
  }
  y[2713] += A[6793] * x[2712];
  y[2713] += A[6794] * x[2714];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2714] += A[1 * i0 + 6795] * x[1 * i0 + 2713];
  }
  y[2715] += A[6798] * x[2714];
  y[2715] += A[6799] * x[2716];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2716] += A[1 * i0 + 6800] * x[1 * i0 + 2715];
  }
  y[2717] += A[6803] * x[2716];
  y[2717] += A[6804] * x[2718];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2718] += A[1 * i0 + 6805] * x[1 * i0 + 2717];
  }
  y[2719] += A[6808] * x[2718];
  y[2719] += A[6809] * x[2720];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2720] += A[1 * i0 + 6810] * x[1 * i0 + 2719];
  }
  y[2721] += A[6813] * x[2720];
  y[2721] += A[6814] * x[2722];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2722] += A[1 * i0 + 6815] * x[1 * i0 + 2721];
  }
  y[2723] += A[6818] * x[2722];
  y[2723] += A[6819] * x[2724];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2724] += A[1 * i0 + 6820] * x[1 * i0 + 2723];
  }
  y[2725] += A[6823] * x[2724];
  y[2725] += A[6824] * x[2726];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2726] += A[1 * i0 + 6825] * x[1 * i0 + 2725];
  }
  y[2727] += A[6828] * x[2726];
  y[2727] += A[6829] * x[2728];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2728] += A[1 * i0 + 6830] * x[1 * i0 + 2727];
  }
  y[2729] += A[6833] * x[2728];
  y[2729] += A[6834] * x[2730];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2730] += A[1 * i0 + 6835] * x[1 * i0 + 2729];
  }
  y[2731] += A[6838] * x[9];
  y[2731] += A[6839] * x[2730];
  y[2731] += A[6840] * x[4019];
  y[2732] += A[6841] * x[2572];
  y[2732] += A[6842] * x[2733];
  y[2732] += A[6843] * x[4020];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2733] += A[1 * i0 + 6844] * x[1 * i0 + 2732];
  }
  y[2734] += A[6847] * x[2733];
  y[2734] += A[6848] * x[2735];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2735] += A[1 * i0 + 6849] * x[1 * i0 + 2734];
  }
  y[2736] += A[6852] * x[2735];
  y[2736] += A[6853] * x[2737];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2737] += A[1 * i0 + 6854] * x[1 * i0 + 2736];
  }
  y[2738] += A[6857] * x[2737];
  y[2738] += A[6858] * x[2739];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2739] += A[1 * i0 + 6859] * x[1 * i0 + 2738];
  }
  y[2740] += A[6862] * x[2739];
  y[2740] += A[6863] * x[2741];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2741] += A[1 * i0 + 6864] * x[1 * i0 + 2740];
  }
  y[2742] += A[6867] * x[2741];
  y[2742] += A[6868] * x[2743];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2743] += A[1 * i0 + 6869] * x[1 * i0 + 2742];
  }
  y[2744] += A[6872] * x[2743];
  y[2744] += A[6873] * x[2745];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2745] += A[1 * i0 + 6874] * x[1 * i0 + 2744];
  }
  y[2746] += A[6877] * x[2745];
  y[2746] += A[6878] * x[2747];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2747] += A[1 * i0 + 6879] * x[1 * i0 + 2746];
  }
  y[2748] += A[6882] * x[2747];
  y[2748] += A[6883] * x[2749];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2749] += A[1 * i0 + 6884] * x[1 * i0 + 2748];
  }
  y[2750] += A[6887] * x[2749];
  y[2750] += A[6888] * x[2751];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2751] += A[1 * i0 + 6889] * x[1 * i0 + 2750];
  }
  y[2752] += A[6892] * x[2751];
  y[2752] += A[6893] * x[2753];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2753] += A[1 * i0 + 6894] * x[1 * i0 + 2752];
  }
  y[2754] += A[6897] * x[2753];
  y[2754] += A[6898] * x[2755];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2755] += A[1 * i0 + 6899] * x[1 * i0 + 2754];
  }
  y[2756] += A[6902] * x[2755];
  y[2756] += A[6903] * x[2757];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2757] += A[1 * i0 + 6904] * x[1 * i0 + 2756];
  }
  y[2758] += A[6907] * x[2757];
  y[2758] += A[6908] * x[2759];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2759] += A[1 * i0 + 6909] * x[1 * i0 + 2758];
  }
  y[2760] += A[6912] * x[2759];
  y[2760] += A[6913] * x[2761];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2761] += A[1 * i0 + 6914] * x[1 * i0 + 2760];
  }
  y[2762] += A[6917] * x[2761];
  y[2762] += A[6918] * x[2763];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2763] += A[1 * i0 + 6919] * x[1 * i0 + 2762];
  }
  y[2764] += A[6922] * x[2763];
  y[2764] += A[6923] * x[2765];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2765] += A[1 * i0 + 6924] * x[1 * i0 + 2764];
  }
  y[2766] += A[6927] * x[2765];
  y[2766] += A[6928] * x[2767];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2767] += A[1 * i0 + 6929] * x[1 * i0 + 2766];
  }
  y[2768] += A[6932] * x[2767];
  y[2768] += A[6933] * x[2769];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2769] += A[1 * i0 + 6934] * x[1 * i0 + 2768];
  }
  y[2770] += A[6937] * x[2769];
  y[2770] += A[6938] * x[2771];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2771] += A[1 * i0 + 6939] * x[1 * i0 + 2770];
  }
  y[2772] += A[6942] * x[2771];
  y[2772] += A[6943] * x[2773];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2773] += A[1 * i0 + 6944] * x[1 * i0 + 2772];
  }
  y[2774] += A[6947] * x[2773];
  y[2774] += A[6948] * x[2775];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2775] += A[1 * i0 + 6949] * x[1 * i0 + 2774];
  }
  y[2776] += A[6952] * x[2775];
  y[2776] += A[6953] * x[2777];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2777] += A[1 * i0 + 6954] * x[1 * i0 + 2776];
  }
  y[2778] += A[6957] * x[2777];
  y[2778] += A[6958] * x[2779];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2779] += A[1 * i0 + 6959] * x[1 * i0 + 2778];
  }
  y[2780] += A[6962] * x[2779];
  y[2780] += A[6963] * x[2781];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2781] += A[1 * i0 + 6964] * x[1 * i0 + 2780];
  }
  y[2782] += A[6967] * x[2781];
  y[2782] += A[6968] * x[2783];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2783] += A[1 * i0 + 6969] * x[1 * i0 + 2782];
  }
  y[2784] += A[6972] * x[2783];
  y[2784] += A[6973] * x[2785];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2785] += A[1 * i0 + 6974] * x[1 * i0 + 2784];
  }
  y[2786] += A[6977] * x[2785];
  y[2786] += A[6978] * x[2787];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2787] += A[1 * i0 + 6979] * x[1 * i0 + 2786];
  }
  y[2788] += A[6982] * x[2787];
  y[2788] += A[6983] * x[2789];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2789] += A[1 * i0 + 6984] * x[1 * i0 + 2788];
  }
  y[2790] += A[6987] * x[2789];
  y[2790] += A[6988] * x[2791];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2791] += A[1 * i0 + 6989] * x[1 * i0 + 2790];
  }
  y[2792] += A[6992] * x[2791];
  y[2792] += A[6993] * x[2793];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2793] += A[1 * i0 + 6994] * x[1 * i0 + 2792];
  }
  y[2794] += A[6997] * x[2793];
  y[2794] += A[6998] * x[2795];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2795] += A[1 * i0 + 6999] * x[1 * i0 + 2794];
  }
  y[2796] += A[7002] * x[2795];
  y[2796] += A[7003] * x[2797];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2797] += A[1 * i0 + 7004] * x[1 * i0 + 2796];
  }
  y[2798] += A[7007] * x[2797];
  y[2798] += A[7008] * x[2799];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2799] += A[1 * i0 + 7009] * x[1 * i0 + 2798];
  }
  y[2800] += A[7012] * x[2799];
  y[2800] += A[7013] * x[2801];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2801] += A[1 * i0 + 7014] * x[1 * i0 + 2800];
  }
  y[2802] += A[7017] * x[2801];
  y[2802] += A[7018] * x[2803];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2803] += A[1 * i0 + 7019] * x[1 * i0 + 2802];
  }
  y[2804] += A[7022] * x[2803];
  y[2804] += A[7023] * x[2805];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2805] += A[1 * i0 + 7024] * x[1 * i0 + 2804];
  }
  y[2806] += A[7027] * x[2805];
  y[2806] += A[7028] * x[2807];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2807] += A[1 * i0 + 7029] * x[1 * i0 + 2806];
  }
  y[2808] += A[7032] * x[2807];
  y[2808] += A[7033] * x[2809];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2809] += A[1 * i0 + 7034] * x[1 * i0 + 2808];
  }
  y[2810] += A[7037] * x[2809];
  y[2810] += A[7038] * x[2811];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2811] += A[1 * i0 + 7039] * x[1 * i0 + 2810];
  }
  y[2812] += A[7042] * x[2811];
  y[2812] += A[7043] * x[2813];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2813] += A[1 * i0 + 7044] * x[1 * i0 + 2812];
  }
  y[2814] += A[7047] * x[2813];
  y[2814] += A[7048] * x[2815];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2815] += A[1 * i0 + 7049] * x[1 * i0 + 2814];
  }
  y[2816] += A[7052] * x[2815];
  y[2816] += A[7053] * x[2817];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2817] += A[1 * i0 + 7054] * x[1 * i0 + 2816];
  }
  y[2818] += A[7057] * x[2817];
  y[2818] += A[7058] * x[2819];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2819] += A[1 * i0 + 7059] * x[1 * i0 + 2818];
  }
  y[2820] += A[7062] * x[2819];
  y[2820] += A[7063] * x[2821];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2821] += A[1 * i0 + 7064] * x[1 * i0 + 2820];
  }
  y[2822] += A[7067] * x[2821];
  y[2822] += A[7068] * x[2823];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2823] += A[1 * i0 + 7069] * x[1 * i0 + 2822];
  }
  y[2824] += A[7072] * x[2823];
  y[2824] += A[7073] * x[2825];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2825] += A[1 * i0 + 7074] * x[1 * i0 + 2824];
  }
  y[2826] += A[7077] * x[2825];
  y[2826] += A[7078] * x[2827];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2827] += A[1 * i0 + 7079] * x[1 * i0 + 2826];
  }
  y[2828] += A[7082] * x[2827];
  y[2828] += A[7083] * x[2829];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2829] += A[1 * i0 + 7084] * x[1 * i0 + 2828];
  }
  y[2830] += A[7087] * x[2829];
  y[2830] += A[7088] * x[2831];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2831] += A[1 * i0 + 7089] * x[1 * i0 + 2830];
  }
  y[2832] += A[7092] * x[2831];
  y[2832] += A[7093] * x[2833];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2833] += A[1 * i0 + 7094] * x[1 * i0 + 2832];
  }
  y[2834] += A[7097] * x[2833];
  y[2834] += A[7098] * x[2835];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2835] += A[1 * i0 + 7099] * x[1 * i0 + 2834];
  }
  y[2836] += A[7102] * x[2835];
  y[2836] += A[7103] * x[2837];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2837] += A[1 * i0 + 7104] * x[1 * i0 + 2836];
  }
  y[2838] += A[7107] * x[2837];
  y[2838] += A[7108] * x[2839];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2839] += A[1 * i0 + 7109] * x[1 * i0 + 2838];
  }
  y[2840] += A[7112] * x[2839];
  y[2840] += A[7113] * x[2841];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2841] += A[1 * i0 + 7114] * x[1 * i0 + 2840];
  }
  y[2842] += A[7117] * x[2841];
  y[2842] += A[7118] * x[2843];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2843] += A[1 * i0 + 7119] * x[1 * i0 + 2842];
  }
  y[2844] += A[7122] * x[2843];
  y[2844] += A[7123] * x[2845];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2845] += A[1 * i0 + 7124] * x[1 * i0 + 2844];
  }
  y[2846] += A[7127] * x[2845];
  y[2846] += A[7128] * x[2847];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2847] += A[1 * i0 + 7129] * x[1 * i0 + 2846];
  }
  y[2848] += A[7132] * x[2847];
  y[2848] += A[7133] * x[2849];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2849] += A[1 * i0 + 7134] * x[1 * i0 + 2848];
  }
  y[2850] += A[7137] * x[2849];
  y[2850] += A[7138] * x[2851];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2851] += A[1 * i0 + 7139] * x[1 * i0 + 2850];
  }
  y[2852] += A[7142] * x[2851];
  y[2852] += A[7143] * x[2853];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2853] += A[1 * i0 + 7144] * x[1 * i0 + 2852];
  }
  y[2854] += A[7147] * x[2853];
  y[2854] += A[7148] * x[2855];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2855] += A[1 * i0 + 7149] * x[1 * i0 + 2854];
  }
  y[2856] += A[7152] * x[2855];
  y[2856] += A[7153] * x[2857];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2857] += A[1 * i0 + 7154] * x[1 * i0 + 2856];
  }
  y[2858] += A[7157] * x[2857];
  y[2858] += A[7158] * x[2859];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2859] += A[1 * i0 + 7159] * x[1 * i0 + 2858];
  }
  y[2860] += A[7162] * x[2859];
  y[2860] += A[7163] * x[2861];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2861] += A[1 * i0 + 7164] * x[1 * i0 + 2860];
  }
  y[2862] += A[7167] * x[2861];
  y[2862] += A[7168] * x[2863];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2863] += A[1 * i0 + 7169] * x[1 * i0 + 2862];
  }
  y[2864] += A[7172] * x[2863];
  y[2864] += A[7173] * x[2865];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2865] += A[1 * i0 + 7174] * x[1 * i0 + 2864];
  }
  y[2866] += A[7177] * x[2865];
  y[2866] += A[7178] * x[2867];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2867] += A[1 * i0 + 7179] * x[1 * i0 + 2866];
  }
  y[2868] += A[7182] * x[2867];
  y[2868] += A[7183] * x[2869];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2869] += A[1 * i0 + 7184] * x[1 * i0 + 2868];
  }
  y[2870] += A[7187] * x[2869];
  y[2870] += A[7188] * x[2871];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2871] += A[1 * i0 + 7189] * x[1 * i0 + 2870];
  }
  y[2872] += A[7192] * x[2871];
  y[2872] += A[7193] * x[2873];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2873] += A[1 * i0 + 7194] * x[1 * i0 + 2872];
  }
  y[2874] += A[7197] * x[2873];
  y[2874] += A[7198] * x[2875];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2875] += A[1 * i0 + 7199] * x[1 * i0 + 2874];
  }
  y[2876] += A[7202] * x[2875];
  y[2876] += A[7203] * x[2877];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2877] += A[1 * i0 + 7204] * x[1 * i0 + 2876];
  }
  y[2878] += A[7207] * x[2877];
  y[2878] += A[7208] * x[2879];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2879] += A[1 * i0 + 7209] * x[1 * i0 + 2878];
  }
  y[2880] += A[7212] * x[2879];
  y[2880] += A[7213] * x[2881];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2881] += A[1 * i0 + 7214] * x[1 * i0 + 2880];
  }
  y[2882] += A[7217] * x[2881];
  y[2882] += A[7218] * x[2883];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2883] += A[1 * i0 + 7219] * x[1 * i0 + 2882];
  }
  y[2884] += A[7222] * x[2883];
  y[2884] += A[7223] * x[2885];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2885] += A[1 * i0 + 7224] * x[1 * i0 + 2884];
  }
  y[2886] += A[7227] * x[2885];
  y[2886] += A[7228] * x[2887];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2887] += A[1 * i0 + 7229] * x[1 * i0 + 2886];
  }
  y[2888] += A[7232] * x[2887];
  y[2888] += A[7233] * x[2889];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2889] += A[1 * i0 + 7234] * x[1 * i0 + 2888];
  }
  y[2890] += A[7237] * x[2889];
  y[2890] += A[7238] * x[2891];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2891] += A[1 * i0 + 7239] * x[1 * i0 + 2890];
  }
  y[2892] += A[7242] * x[2891];
  y[2892] += A[7243] * x[4021];
  y[2893] += A[7244] * x[2894];
  y[2893] += A[7245] * x[4022];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2894] += A[1 * i0 + 7246] * x[1 * i0 + 2893];
  }
  y[2895] += A[7249] * x[2894];
  y[2895] += A[7250] * x[2896];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2896] += A[1 * i0 + 7251] * x[1 * i0 + 2895];
  }
  y[2897] += A[7254] * x[2896];
  y[2897] += A[7255] * x[2898];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2898] += A[1 * i0 + 7256] * x[1 * i0 + 2897];
  }
  y[2899] += A[7259] * x[2898];
  y[2899] += A[7260] * x[2900];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2900] += A[1 * i0 + 7261] * x[1 * i0 + 2899];
  }
  y[2901] += A[7264] * x[2900];
  y[2901] += A[7265] * x[2902];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2902] += A[1 * i0 + 7266] * x[1 * i0 + 2901];
  }
  y[2903] += A[7269] * x[2902];
  y[2903] += A[7270] * x[2904];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2904] += A[1 * i0 + 7271] * x[1 * i0 + 2903];
  }
  y[2905] += A[7274] * x[2904];
  y[2905] += A[7275] * x[2906];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2906] += A[1 * i0 + 7276] * x[1 * i0 + 2905];
  }
  y[2907] += A[7279] * x[2906];
  y[2907] += A[7280] * x[2908];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2908] += A[1 * i0 + 7281] * x[1 * i0 + 2907];
  }
  y[2909] += A[7284] * x[2908];
  y[2909] += A[7285] * x[2910];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2910] += A[1 * i0 + 7286] * x[1 * i0 + 2909];
  }
  y[2911] += A[7289] * x[2910];
  y[2911] += A[7290] * x[2912];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2912] += A[1 * i0 + 7291] * x[1 * i0 + 2911];
  }
  y[2913] += A[7294] * x[2912];
  y[2913] += A[7295] * x[2914];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2914] += A[1 * i0 + 7296] * x[1 * i0 + 2913];
  }
  y[2915] += A[7299] * x[2914];
  y[2915] += A[7300] * x[2916];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2916] += A[1 * i0 + 7301] * x[1 * i0 + 2915];
  }
  y[2917] += A[7304] * x[2916];
  y[2917] += A[7305] * x[2918];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2918] += A[1 * i0 + 7306] * x[1 * i0 + 2917];
  }
  y[2919] += A[7309] * x[2918];
  y[2919] += A[7310] * x[2920];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2920] += A[1 * i0 + 7311] * x[1 * i0 + 2919];
  }
  y[2921] += A[7314] * x[2920];
  y[2921] += A[7315] * x[2922];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2922] += A[1 * i0 + 7316] * x[1 * i0 + 2921];
  }
  y[2923] += A[7319] * x[2922];
  y[2923] += A[7320] * x[2924];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2924] += A[1 * i0 + 7321] * x[1 * i0 + 2923];
  }
  y[2925] += A[7324] * x[2924];
  y[2925] += A[7325] * x[2926];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2926] += A[1 * i0 + 7326] * x[1 * i0 + 2925];
  }
  y[2927] += A[7329] * x[2926];
  y[2927] += A[7330] * x[2928];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2928] += A[1 * i0 + 7331] * x[1 * i0 + 2927];
  }
  y[2929] += A[7334] * x[2928];
  y[2929] += A[7335] * x[2930];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2930] += A[1 * i0 + 7336] * x[1 * i0 + 2929];
  }
  y[2931] += A[7339] * x[2930];
  y[2931] += A[7340] * x[2932];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2932] += A[1 * i0 + 7341] * x[1 * i0 + 2931];
  }
  y[2933] += A[7344] * x[2932];
  y[2933] += A[7345] * x[2934];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2934] += A[1 * i0 + 7346] * x[1 * i0 + 2933];
  }
  y[2935] += A[7349] * x[2934];
  y[2935] += A[7350] * x[2936];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2936] += A[1 * i0 + 7351] * x[1 * i0 + 2935];
  }
  y[2937] += A[7354] * x[2936];
  y[2937] += A[7355] * x[2938];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2938] += A[1 * i0 + 7356] * x[1 * i0 + 2937];
  }
  y[2939] += A[7359] * x[2938];
  y[2939] += A[7360] * x[2940];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2940] += A[1 * i0 + 7361] * x[1 * i0 + 2939];
  }
  y[2941] += A[7364] * x[2940];
  y[2941] += A[7365] * x[2942];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2942] += A[1 * i0 + 7366] * x[1 * i0 + 2941];
  }
  y[2943] += A[7369] * x[2942];
  y[2943] += A[7370] * x[2944];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2944] += A[1 * i0 + 7371] * x[1 * i0 + 2943];
  }
  y[2945] += A[7374] * x[2944];
  y[2945] += A[7375] * x[2946];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2946] += A[1 * i0 + 7376] * x[1 * i0 + 2945];
  }
  y[2947] += A[7379] * x[2946];
  y[2947] += A[7380] * x[2948];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2948] += A[1 * i0 + 7381] * x[1 * i0 + 2947];
  }
  y[2949] += A[7384] * x[2948];
  y[2949] += A[7385] * x[2950];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2950] += A[1 * i0 + 7386] * x[1 * i0 + 2949];
  }
  y[2951] += A[7389] * x[2950];
  y[2951] += A[7390] * x[2952];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2952] += A[1 * i0 + 7391] * x[1 * i0 + 2951];
  }
  y[2953] += A[7394] * x[2952];
  y[2953] += A[7395] * x[2954];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2954] += A[1 * i0 + 7396] * x[1 * i0 + 2953];
  }
  y[2955] += A[7399] * x[2954];
  y[2955] += A[7400] * x[2956];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2956] += A[1 * i0 + 7401] * x[1 * i0 + 2955];
  }
  y[2957] += A[7404] * x[2956];
  y[2957] += A[7405] * x[2958];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2958] += A[1 * i0 + 7406] * x[1 * i0 + 2957];
  }
  y[2959] += A[7409] * x[2958];
  y[2959] += A[7410] * x[2960];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2960] += A[1 * i0 + 7411] * x[1 * i0 + 2959];
  }
  y[2961] += A[7414] * x[2960];
  y[2961] += A[7415] * x[2962];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2962] += A[1 * i0 + 7416] * x[1 * i0 + 2961];
  }
  y[2963] += A[7419] * x[2962];
  y[2963] += A[7420] * x[2964];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2964] += A[1 * i0 + 7421] * x[1 * i0 + 2963];
  }
  y[2965] += A[7424] * x[2964];
  y[2965] += A[7425] * x[2966];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2966] += A[1 * i0 + 7426] * x[1 * i0 + 2965];
  }
  y[2967] += A[7429] * x[2966];
  y[2967] += A[7430] * x[2968];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2968] += A[1 * i0 + 7431] * x[1 * i0 + 2967];
  }
  y[2969] += A[7434] * x[2968];
  y[2969] += A[7435] * x[2970];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2970] += A[1 * i0 + 7436] * x[1 * i0 + 2969];
  }
  y[2971] += A[7439] * x[2970];
  y[2971] += A[7440] * x[2972];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2972] += A[1 * i0 + 7441] * x[1 * i0 + 2971];
  }
  y[2973] += A[7444] * x[2972];
  y[2973] += A[7445] * x[2974];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2974] += A[1 * i0 + 7446] * x[1 * i0 + 2973];
  }
  y[2975] += A[7449] * x[2974];
  y[2975] += A[7450] * x[2976];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2976] += A[1 * i0 + 7451] * x[1 * i0 + 2975];
  }
  y[2977] += A[7454] * x[2976];
  y[2977] += A[7455] * x[2978];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2978] += A[1 * i0 + 7456] * x[1 * i0 + 2977];
  }
  y[2979] += A[7459] * x[2978];
  y[2979] += A[7460] * x[2980];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2980] += A[1 * i0 + 7461] * x[1 * i0 + 2979];
  }
  y[2981] += A[7464] * x[2980];
  y[2981] += A[7465] * x[2982];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2982] += A[1 * i0 + 7466] * x[1 * i0 + 2981];
  }
  y[2983] += A[7469] * x[2982];
  y[2983] += A[7470] * x[2984];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2984] += A[1 * i0 + 7471] * x[1 * i0 + 2983];
  }
  y[2985] += A[7474] * x[2984];
  y[2985] += A[7475] * x[2986];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2986] += A[1 * i0 + 7476] * x[1 * i0 + 2985];
  }
  y[2987] += A[7479] * x[2986];
  y[2987] += A[7480] * x[2988];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2988] += A[1 * i0 + 7481] * x[1 * i0 + 2987];
  }
  y[2989] += A[7484] * x[2988];
  y[2989] += A[7485] * x[2990];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2990] += A[1 * i0 + 7486] * x[1 * i0 + 2989];
  }
  y[2991] += A[7489] * x[2990];
  y[2991] += A[7490] * x[2992];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2992] += A[1 * i0 + 7491] * x[1 * i0 + 2991];
  }
  y[2993] += A[7494] * x[2992];
  y[2993] += A[7495] * x[2994];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2994] += A[1 * i0 + 7496] * x[1 * i0 + 2993];
  }
  y[2995] += A[7499] * x[2994];
  y[2995] += A[7500] * x[2996];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2996] += A[1 * i0 + 7501] * x[1 * i0 + 2995];
  }
  y[2997] += A[7504] * x[2996];
  y[2997] += A[7505] * x[2998];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 2998] += A[1 * i0 + 7506] * x[1 * i0 + 2997];
  }
  y[2999] += A[7509] * x[2998];
  y[2999] += A[7510] * x[3000];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3000] += A[1 * i0 + 7511] * x[1 * i0 + 2999];
  }
  y[3001] += A[7514] * x[3000];
  y[3001] += A[7515] * x[3002];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3002] += A[1 * i0 + 7516] * x[1 * i0 + 3001];
  }
  y[3003] += A[7519] * x[3002];
  y[3003] += A[7520] * x[3004];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3004] += A[1 * i0 + 7521] * x[1 * i0 + 3003];
  }
  y[3005] += A[7524] * x[3004];
  y[3005] += A[7525] * x[3006];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3006] += A[1 * i0 + 7526] * x[1 * i0 + 3005];
  }
  y[3007] += A[7529] * x[3006];
  y[3007] += A[7530] * x[3008];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3008] += A[1 * i0 + 7531] * x[1 * i0 + 3007];
  }
  y[3009] += A[7534] * x[3008];
  y[3009] += A[7535] * x[3010];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3010] += A[1 * i0 + 7536] * x[1 * i0 + 3009];
  }
  y[3011] += A[7539] * x[3010];
  y[3011] += A[7540] * x[3012];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3012] += A[1 * i0 + 7541] * x[1 * i0 + 3011];
  }
  y[3013] += A[7544] * x[3012];
  y[3013] += A[7545] * x[3014];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3014] += A[1 * i0 + 7546] * x[1 * i0 + 3013];
  }
  y[3015] += A[7549] * x[3014];
  y[3015] += A[7550] * x[3016];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3016] += A[1 * i0 + 7551] * x[1 * i0 + 3015];
  }
  y[3017] += A[7554] * x[3016];
  y[3017] += A[7555] * x[3018];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3018] += A[1 * i0 + 7556] * x[1 * i0 + 3017];
  }
  y[3019] += A[7559] * x[3018];
  y[3019] += A[7560] * x[3020];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3020] += A[1 * i0 + 7561] * x[1 * i0 + 3019];
  }
  y[3021] += A[7564] * x[3020];
  y[3021] += A[7565] * x[3022];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3022] += A[1 * i0 + 7566] * x[1 * i0 + 3021];
  }
  y[3023] += A[7569] * x[3022];
  y[3023] += A[7570] * x[3024];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3024] += A[1 * i0 + 7571] * x[1 * i0 + 3023];
  }
  y[3025] += A[7574] * x[3024];
  y[3025] += A[7575] * x[3026];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3026] += A[1 * i0 + 7576] * x[1 * i0 + 3025];
  }
  y[3027] += A[7579] * x[3026];
  y[3027] += A[7580] * x[3028];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3028] += A[1 * i0 + 7581] * x[1 * i0 + 3027];
  }
  y[3029] += A[7584] * x[3028];
  y[3029] += A[7585] * x[3030];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3030] += A[1 * i0 + 7586] * x[1 * i0 + 3029];
  }
  y[3031] += A[7589] * x[3030];
  y[3031] += A[7590] * x[3032];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3032] += A[1 * i0 + 7591] * x[1 * i0 + 3031];
  }
  y[3033] += A[7594] * x[3032];
  y[3033] += A[7595] * x[3034];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3034] += A[1 * i0 + 7596] * x[1 * i0 + 3033];
  }
}

void kernel_spmv_fragment_6(double *restrict A, double *restrict x,
                            double *restrict y) {
  register int i0, i1, i2, i3, i4, i5, i6;

  y[3035] += A[7599] * x[3034];
  y[3035] += A[7600] * x[3036];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3036] += A[1 * i0 + 7601] * x[1 * i0 + 3035];
  }
  y[3037] += A[7604] * x[3036];
  y[3037] += A[7605] * x[3038];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3038] += A[1 * i0 + 7606] * x[1 * i0 + 3037];
  }
  y[3039] += A[7609] * x[3038];
  y[3039] += A[7610] * x[3040];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3040] += A[1 * i0 + 7611] * x[1 * i0 + 3039];
  }
  y[3041] += A[7614] * x[3040];
  y[3041] += A[7615] * x[3042];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3042] += A[1 * i0 + 7616] * x[1 * i0 + 3041];
  }
  y[3043] += A[7619] * x[3042];
  y[3043] += A[7620] * x[3044];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3044] += A[1 * i0 + 7621] * x[1 * i0 + 3043];
  }
  y[3045] += A[7624] * x[3044];
  y[3045] += A[7625] * x[3046];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3046] += A[1 * i0 + 7626] * x[1 * i0 + 3045];
  }
  y[3047] += A[7629] * x[3046];
  y[3047] += A[7630] * x[3048];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3048] += A[1 * i0 + 7631] * x[1 * i0 + 3047];
  }
  y[3049] += A[7634] * x[3048];
  y[3049] += A[7635] * x[3050];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3050] += A[1 * i0 + 7636] * x[1 * i0 + 3049];
  }
  y[3051] += A[7639] * x[3050];
  y[3051] += A[7640] * x[3052];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 3052] += A[1 * i0 + 7641] * x[1 * i0 + 3051];
  }
  y[3052] += A[7643] * x[3212];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 3053] += A[1 * i0 + 7644] * x[1 * i0 + 3053];
  }
  y[3053] += A[7646] * x[3213];
  y[3054] += A[7647] * x[3053];
  y[3054] += A[7648] * x[3055];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3055] += A[1 * i0 + 7649] * x[1 * i0 + 3054];
  }
  y[3056] += A[7652] * x[3055];
  y[3056] += A[7653] * x[3057];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3057] += A[1 * i0 + 7654] * x[1 * i0 + 3056];
  }
  y[3058] += A[7657] * x[3057];
  y[3058] += A[7658] * x[3059];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3059] += A[1 * i0 + 7659] * x[1 * i0 + 3058];
  }
  y[3060] += A[7662] * x[3059];
  y[3060] += A[7663] * x[3061];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3061] += A[1 * i0 + 7664] * x[1 * i0 + 3060];
  }
  y[3062] += A[7667] * x[3061];
  y[3062] += A[7668] * x[3063];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3063] += A[1 * i0 + 7669] * x[1 * i0 + 3062];
  }
  y[3064] += A[7672] * x[3063];
  y[3064] += A[7673] * x[3065];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3065] += A[1 * i0 + 7674] * x[1 * i0 + 3064];
  }
  y[3066] += A[7677] * x[3065];
  y[3066] += A[7678] * x[3067];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3067] += A[1 * i0 + 7679] * x[1 * i0 + 3066];
  }
  y[3068] += A[7682] * x[3067];
  y[3068] += A[7683] * x[3069];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3069] += A[1 * i0 + 7684] * x[1 * i0 + 3068];
  }
  y[3070] += A[7687] * x[3069];
  y[3070] += A[7688] * x[3071];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3071] += A[1 * i0 + 7689] * x[1 * i0 + 3070];
  }
  y[3072] += A[7692] * x[3071];
  y[3072] += A[7693] * x[3073];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3073] += A[1 * i0 + 7694] * x[1 * i0 + 3072];
  }
  y[3074] += A[7697] * x[3073];
  y[3074] += A[7698] * x[3075];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3075] += A[1 * i0 + 7699] * x[1 * i0 + 3074];
  }
  y[3076] += A[7702] * x[3075];
  y[3076] += A[7703] * x[3077];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3077] += A[1 * i0 + 7704] * x[1 * i0 + 3076];
  }
  y[3078] += A[7707] * x[3077];
  y[3078] += A[7708] * x[3079];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3079] += A[1 * i0 + 7709] * x[1 * i0 + 3078];
  }
  y[3080] += A[7712] * x[3079];
  y[3080] += A[7713] * x[3081];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3081] += A[1 * i0 + 7714] * x[1 * i0 + 3080];
  }
  y[3082] += A[7717] * x[3081];
  y[3082] += A[7718] * x[3083];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3083] += A[1 * i0 + 7719] * x[1 * i0 + 3082];
  }
  y[3084] += A[7722] * x[3083];
  y[3084] += A[7723] * x[3085];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3085] += A[1 * i0 + 7724] * x[1 * i0 + 3084];
  }
  y[3086] += A[7727] * x[3085];
  y[3086] += A[7728] * x[3087];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3087] += A[1 * i0 + 7729] * x[1 * i0 + 3086];
  }
  y[3088] += A[7732] * x[3087];
  y[3088] += A[7733] * x[3089];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3089] += A[1 * i0 + 7734] * x[1 * i0 + 3088];
  }
  y[3090] += A[7737] * x[3089];
  y[3090] += A[7738] * x[3091];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3091] += A[1 * i0 + 7739] * x[1 * i0 + 3090];
  }
  y[3092] += A[7742] * x[3091];
  y[3092] += A[7743] * x[3093];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3093] += A[1 * i0 + 7744] * x[1 * i0 + 3092];
  }
  y[3094] += A[7747] * x[3093];
  y[3094] += A[7748] * x[3095];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3095] += A[1 * i0 + 7749] * x[1 * i0 + 3094];
  }
  y[3096] += A[7752] * x[3095];
  y[3096] += A[7753] * x[3097];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3097] += A[1 * i0 + 7754] * x[1 * i0 + 3096];
  }
  y[3098] += A[7757] * x[3097];
  y[3098] += A[7758] * x[3099];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3099] += A[1 * i0 + 7759] * x[1 * i0 + 3098];
  }
  y[3100] += A[7762] * x[3099];
  y[3100] += A[7763] * x[3101];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3101] += A[1 * i0 + 7764] * x[1 * i0 + 3100];
  }
  y[3102] += A[7767] * x[3101];
  y[3102] += A[7768] * x[3103];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3103] += A[1 * i0 + 7769] * x[1 * i0 + 3102];
  }
  y[3104] += A[7772] * x[3103];
  y[3104] += A[7773] * x[3105];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3105] += A[1 * i0 + 7774] * x[1 * i0 + 3104];
  }
  y[3106] += A[7777] * x[3105];
  y[3106] += A[7778] * x[3107];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3107] += A[1 * i0 + 7779] * x[1 * i0 + 3106];
  }
  y[3108] += A[7782] * x[3107];
  y[3108] += A[7783] * x[3109];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3109] += A[1 * i0 + 7784] * x[1 * i0 + 3108];
  }
  y[3110] += A[7787] * x[3109];
  y[3110] += A[7788] * x[3111];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3111] += A[1 * i0 + 7789] * x[1 * i0 + 3110];
  }
  y[3112] += A[7792] * x[3111];
  y[3112] += A[7793] * x[3113];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3113] += A[1 * i0 + 7794] * x[1 * i0 + 3112];
  }
  y[3114] += A[7797] * x[3113];
  y[3114] += A[7798] * x[3115];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3115] += A[1 * i0 + 7799] * x[1 * i0 + 3114];
  }
  y[3116] += A[7802] * x[3115];
  y[3116] += A[7803] * x[3117];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3117] += A[1 * i0 + 7804] * x[1 * i0 + 3116];
  }
  y[3118] += A[7807] * x[3117];
  y[3118] += A[7808] * x[3119];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3119] += A[1 * i0 + 7809] * x[1 * i0 + 3118];
  }
  y[3120] += A[7812] * x[3119];
  y[3120] += A[7813] * x[3121];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3121] += A[1 * i0 + 7814] * x[1 * i0 + 3120];
  }
  y[3122] += A[7817] * x[3121];
  y[3122] += A[7818] * x[3123];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3123] += A[1 * i0 + 7819] * x[1 * i0 + 3122];
  }
  y[3124] += A[7822] * x[3123];
  y[3124] += A[7823] * x[3125];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3125] += A[1 * i0 + 7824] * x[1 * i0 + 3124];
  }
  y[3126] += A[7827] * x[3125];
  y[3126] += A[7828] * x[3127];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3127] += A[1 * i0 + 7829] * x[1 * i0 + 3126];
  }
  y[3128] += A[7832] * x[3127];
  y[3128] += A[7833] * x[3129];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3129] += A[1 * i0 + 7834] * x[1 * i0 + 3128];
  }
  y[3130] += A[7837] * x[3129];
  y[3130] += A[7838] * x[3131];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3131] += A[1 * i0 + 7839] * x[1 * i0 + 3130];
  }
  y[3132] += A[7842] * x[3131];
  y[3132] += A[7843] * x[3133];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3133] += A[1 * i0 + 7844] * x[1 * i0 + 3132];
  }
  y[3134] += A[7847] * x[3133];
  y[3134] += A[7848] * x[3135];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3135] += A[1 * i0 + 7849] * x[1 * i0 + 3134];
  }
  y[3136] += A[7852] * x[3135];
  y[3136] += A[7853] * x[3137];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3137] += A[1 * i0 + 7854] * x[1 * i0 + 3136];
  }
  y[3138] += A[7857] * x[3137];
  y[3138] += A[7858] * x[3139];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3139] += A[1 * i0 + 7859] * x[1 * i0 + 3138];
  }
  y[3140] += A[7862] * x[3139];
  y[3140] += A[7863] * x[3141];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3141] += A[1 * i0 + 7864] * x[1 * i0 + 3140];
  }
  y[3142] += A[7867] * x[3141];
  y[3142] += A[7868] * x[3143];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3143] += A[1 * i0 + 7869] * x[1 * i0 + 3142];
  }
  y[3144] += A[7872] * x[3143];
  y[3144] += A[7873] * x[3145];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3145] += A[1 * i0 + 7874] * x[1 * i0 + 3144];
  }
  y[3146] += A[7877] * x[3145];
  y[3146] += A[7878] * x[3147];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3147] += A[1 * i0 + 7879] * x[1 * i0 + 3146];
  }
  y[3148] += A[7882] * x[3147];
  y[3148] += A[7883] * x[3149];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3149] += A[1 * i0 + 7884] * x[1 * i0 + 3148];
  }
  y[3150] += A[7887] * x[3149];
  y[3150] += A[7888] * x[3151];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3151] += A[1 * i0 + 7889] * x[1 * i0 + 3150];
  }
  y[3152] += A[7892] * x[3151];
  y[3152] += A[7893] * x[3153];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3153] += A[1 * i0 + 7894] * x[1 * i0 + 3152];
  }
  y[3154] += A[7897] * x[3153];
  y[3154] += A[7898] * x[3155];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3155] += A[1 * i0 + 7899] * x[1 * i0 + 3154];
  }
  y[3156] += A[7902] * x[3155];
  y[3156] += A[7903] * x[3157];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3157] += A[1 * i0 + 7904] * x[1 * i0 + 3156];
  }
  y[3158] += A[7907] * x[3157];
  y[3158] += A[7908] * x[3159];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3159] += A[1 * i0 + 7909] * x[1 * i0 + 3158];
  }
  y[3160] += A[7912] * x[3159];
  y[3160] += A[7913] * x[3161];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3161] += A[1 * i0 + 7914] * x[1 * i0 + 3160];
  }
  y[3162] += A[7917] * x[3161];
  y[3162] += A[7918] * x[3163];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3163] += A[1 * i0 + 7919] * x[1 * i0 + 3162];
  }
  y[3164] += A[7922] * x[3163];
  y[3164] += A[7923] * x[3165];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3165] += A[1 * i0 + 7924] * x[1 * i0 + 3164];
  }
  y[3166] += A[7927] * x[3165];
  y[3166] += A[7928] * x[3167];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3167] += A[1 * i0 + 7929] * x[1 * i0 + 3166];
  }
  y[3168] += A[7932] * x[3167];
  y[3168] += A[7933] * x[3169];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3169] += A[1 * i0 + 7934] * x[1 * i0 + 3168];
  }
  y[3170] += A[7937] * x[3169];
  y[3170] += A[7938] * x[3171];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3171] += A[1 * i0 + 7939] * x[1 * i0 + 3170];
  }
  y[3172] += A[7942] * x[3171];
  y[3172] += A[7943] * x[3173];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3173] += A[1 * i0 + 7944] * x[1 * i0 + 3172];
  }
  y[3174] += A[7947] * x[3173];
  y[3174] += A[7948] * x[3175];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3175] += A[1 * i0 + 7949] * x[1 * i0 + 3174];
  }
  y[3176] += A[7952] * x[3175];
  y[3176] += A[7953] * x[3177];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3177] += A[1 * i0 + 7954] * x[1 * i0 + 3176];
  }
  y[3178] += A[7957] * x[3177];
  y[3178] += A[7958] * x[3179];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3179] += A[1 * i0 + 7959] * x[1 * i0 + 3178];
  }
  y[3180] += A[7962] * x[3179];
  y[3180] += A[7963] * x[3181];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3181] += A[1 * i0 + 7964] * x[1 * i0 + 3180];
  }
  y[3182] += A[7967] * x[3181];
  y[3182] += A[7968] * x[3183];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3183] += A[1 * i0 + 7969] * x[1 * i0 + 3182];
  }
  y[3184] += A[7972] * x[3183];
  y[3184] += A[7973] * x[3185];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3185] += A[1 * i0 + 7974] * x[1 * i0 + 3184];
  }
  y[3186] += A[7977] * x[3185];
  y[3186] += A[7978] * x[3187];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3187] += A[1 * i0 + 7979] * x[1 * i0 + 3186];
  }
  y[3188] += A[7982] * x[3187];
  y[3188] += A[7983] * x[3189];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3189] += A[1 * i0 + 7984] * x[1 * i0 + 3188];
  }
  y[3190] += A[7987] * x[3189];
  y[3190] += A[7988] * x[3191];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3191] += A[1 * i0 + 7989] * x[1 * i0 + 3190];
  }
  y[3192] += A[7992] * x[3191];
  y[3192] += A[7993] * x[3193];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3193] += A[1 * i0 + 7994] * x[1 * i0 + 3192];
  }
  y[3194] += A[7997] * x[3193];
  y[3194] += A[7998] * x[3195];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3195] += A[1 * i0 + 7999] * x[1 * i0 + 3194];
  }
  y[3196] += A[8002] * x[3195];
  y[3196] += A[8003] * x[3197];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3197] += A[1 * i0 + 8004] * x[1 * i0 + 3196];
  }
  y[3198] += A[8007] * x[3197];
  y[3198] += A[8008] * x[3199];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3199] += A[1 * i0 + 8009] * x[1 * i0 + 3198];
  }
  y[3200] += A[8012] * x[3199];
  y[3200] += A[8013] * x[3201];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3201] += A[1 * i0 + 8014] * x[1 * i0 + 3200];
  }
  y[3202] += A[8017] * x[3201];
  y[3202] += A[8018] * x[3203];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3203] += A[1 * i0 + 8019] * x[1 * i0 + 3202];
  }
  y[3204] += A[8022] * x[3203];
  y[3204] += A[8023] * x[3205];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3205] += A[1 * i0 + 8024] * x[1 * i0 + 3204];
  }
  y[3206] += A[8027] * x[3205];
  y[3206] += A[8028] * x[3207];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3207] += A[1 * i0 + 8029] * x[1 * i0 + 3206];
  }
  y[3208] += A[8032] * x[3207];
  y[3208] += A[8033] * x[3209];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3209] += A[1 * i0 + 8034] * x[1 * i0 + 3208];
  }
  y[3210] += A[8037] * x[3209];
  y[3210] += A[8038] * x[3211];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3211] += A[1 * i0 + 8039] * x[1 * i0 + 3210];
  }
  y[3212] += A[8042] * x[10];
  y[3212] += A[8043] * x[3052];
  y[3212] += A[8044] * x[3211];
  y[3212] += A[8045] * x[4023];
  y[3213] += A[8046] * x[3053];
  y[3213] += A[8047] * x[3214];
  y[3213] += A[8048] * x[4024];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3214] += A[1 * i0 + 8049] * x[1 * i0 + 3213];
  }
  y[3215] += A[8052] * x[3214];
  y[3215] += A[8053] * x[3216];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3216] += A[1 * i0 + 8054] * x[1 * i0 + 3215];
  }
  y[3217] += A[8057] * x[3216];
  y[3217] += A[8058] * x[3218];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3218] += A[1 * i0 + 8059] * x[1 * i0 + 3217];
  }
  y[3219] += A[8062] * x[3218];
  y[3219] += A[8063] * x[3220];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3220] += A[1 * i0 + 8064] * x[1 * i0 + 3219];
  }
  y[3221] += A[8067] * x[3220];
  y[3221] += A[8068] * x[3222];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3222] += A[1 * i0 + 8069] * x[1 * i0 + 3221];
  }
  y[3223] += A[8072] * x[3222];
  y[3223] += A[8073] * x[3224];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3224] += A[1 * i0 + 8074] * x[1 * i0 + 3223];
  }
  y[3225] += A[8077] * x[3224];
  y[3225] += A[8078] * x[3226];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3226] += A[1 * i0 + 8079] * x[1 * i0 + 3225];
  }
  y[3227] += A[8082] * x[3226];
  y[3227] += A[8083] * x[3228];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3228] += A[1 * i0 + 8084] * x[1 * i0 + 3227];
  }
  y[3229] += A[8087] * x[3228];
  y[3229] += A[8088] * x[3230];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3230] += A[1 * i0 + 8089] * x[1 * i0 + 3229];
  }
  y[3231] += A[8092] * x[3230];
  y[3231] += A[8093] * x[3232];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3232] += A[1 * i0 + 8094] * x[1 * i0 + 3231];
  }
  y[3233] += A[8097] * x[3232];
  y[3233] += A[8098] * x[3234];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3234] += A[1 * i0 + 8099] * x[1 * i0 + 3233];
  }
  y[3235] += A[8102] * x[3234];
  y[3235] += A[8103] * x[3236];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3236] += A[1 * i0 + 8104] * x[1 * i0 + 3235];
  }
  y[3237] += A[8107] * x[3236];
  y[3237] += A[8108] * x[3238];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3238] += A[1 * i0 + 8109] * x[1 * i0 + 3237];
  }
  y[3239] += A[8112] * x[3238];
  y[3239] += A[8113] * x[3240];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3240] += A[1 * i0 + 8114] * x[1 * i0 + 3239];
  }
  y[3241] += A[8117] * x[3240];
  y[3241] += A[8118] * x[3242];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3242] += A[1 * i0 + 8119] * x[1 * i0 + 3241];
  }
  y[3243] += A[8122] * x[3242];
  y[3243] += A[8123] * x[3244];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3244] += A[1 * i0 + 8124] * x[1 * i0 + 3243];
  }
  y[3245] += A[8127] * x[3244];
  y[3245] += A[8128] * x[3246];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3246] += A[1 * i0 + 8129] * x[1 * i0 + 3245];
  }
  y[3247] += A[8132] * x[3246];
  y[3247] += A[8133] * x[3248];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3248] += A[1 * i0 + 8134] * x[1 * i0 + 3247];
  }
  y[3249] += A[8137] * x[3248];
  y[3249] += A[8138] * x[3250];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3250] += A[1 * i0 + 8139] * x[1 * i0 + 3249];
  }
  y[3251] += A[8142] * x[3250];
  y[3251] += A[8143] * x[3252];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3252] += A[1 * i0 + 8144] * x[1 * i0 + 3251];
  }
  y[3253] += A[8147] * x[3252];
  y[3253] += A[8148] * x[3254];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3254] += A[1 * i0 + 8149] * x[1 * i0 + 3253];
  }
  y[3255] += A[8152] * x[3254];
  y[3255] += A[8153] * x[3256];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3256] += A[1 * i0 + 8154] * x[1 * i0 + 3255];
  }
  y[3257] += A[8157] * x[3256];
  y[3257] += A[8158] * x[3258];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3258] += A[1 * i0 + 8159] * x[1 * i0 + 3257];
  }
  y[3259] += A[8162] * x[3258];
  y[3259] += A[8163] * x[3260];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3260] += A[1 * i0 + 8164] * x[1 * i0 + 3259];
  }
  y[3261] += A[8167] * x[3260];
  y[3261] += A[8168] * x[3262];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3262] += A[1 * i0 + 8169] * x[1 * i0 + 3261];
  }
  y[3263] += A[8172] * x[3262];
  y[3263] += A[8173] * x[3264];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3264] += A[1 * i0 + 8174] * x[1 * i0 + 3263];
  }
  y[3265] += A[8177] * x[3264];
  y[3265] += A[8178] * x[3266];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3266] += A[1 * i0 + 8179] * x[1 * i0 + 3265];
  }
  y[3267] += A[8182] * x[3266];
  y[3267] += A[8183] * x[3268];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3268] += A[1 * i0 + 8184] * x[1 * i0 + 3267];
  }
  y[3269] += A[8187] * x[3268];
  y[3269] += A[8188] * x[3270];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3270] += A[1 * i0 + 8189] * x[1 * i0 + 3269];
  }
  y[3271] += A[8192] * x[3270];
  y[3271] += A[8193] * x[3272];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3272] += A[1 * i0 + 8194] * x[1 * i0 + 3271];
  }
  y[3273] += A[8197] * x[3272];
  y[3273] += A[8198] * x[3274];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3274] += A[1 * i0 + 8199] * x[1 * i0 + 3273];
  }
  y[3275] += A[8202] * x[3274];
  y[3275] += A[8203] * x[3276];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3276] += A[1 * i0 + 8204] * x[1 * i0 + 3275];
  }
  y[3277] += A[8207] * x[3276];
  y[3277] += A[8208] * x[3278];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3278] += A[1 * i0 + 8209] * x[1 * i0 + 3277];
  }
  y[3279] += A[8212] * x[3278];
  y[3279] += A[8213] * x[3280];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3280] += A[1 * i0 + 8214] * x[1 * i0 + 3279];
  }
  y[3281] += A[8217] * x[3280];
  y[3281] += A[8218] * x[3282];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3282] += A[1 * i0 + 8219] * x[1 * i0 + 3281];
  }
  y[3283] += A[8222] * x[3282];
  y[3283] += A[8223] * x[3284];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3284] += A[1 * i0 + 8224] * x[1 * i0 + 3283];
  }
  y[3285] += A[8227] * x[3284];
  y[3285] += A[8228] * x[3286];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3286] += A[1 * i0 + 8229] * x[1 * i0 + 3285];
  }
  y[3287] += A[8232] * x[3286];
  y[3287] += A[8233] * x[3288];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3288] += A[1 * i0 + 8234] * x[1 * i0 + 3287];
  }
  y[3289] += A[8237] * x[3288];
  y[3289] += A[8238] * x[3290];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3290] += A[1 * i0 + 8239] * x[1 * i0 + 3289];
  }
  y[3291] += A[8242] * x[3290];
  y[3291] += A[8243] * x[3292];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3292] += A[1 * i0 + 8244] * x[1 * i0 + 3291];
  }
  y[3293] += A[8247] * x[3292];
  y[3293] += A[8248] * x[3294];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3294] += A[1 * i0 + 8249] * x[1 * i0 + 3293];
  }
  y[3295] += A[8252] * x[3294];
  y[3295] += A[8253] * x[3296];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3296] += A[1 * i0 + 8254] * x[1 * i0 + 3295];
  }
  y[3297] += A[8257] * x[3296];
  y[3297] += A[8258] * x[3298];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3298] += A[1 * i0 + 8259] * x[1 * i0 + 3297];
  }
  y[3299] += A[8262] * x[3298];
  y[3299] += A[8263] * x[3300];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3300] += A[1 * i0 + 8264] * x[1 * i0 + 3299];
  }
  y[3301] += A[8267] * x[3300];
  y[3301] += A[8268] * x[3302];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3302] += A[1 * i0 + 8269] * x[1 * i0 + 3301];
  }
  y[3303] += A[8272] * x[3302];
  y[3303] += A[8273] * x[3304];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3304] += A[1 * i0 + 8274] * x[1 * i0 + 3303];
  }
  y[3305] += A[8277] * x[3304];
  y[3305] += A[8278] * x[3306];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3306] += A[1 * i0 + 8279] * x[1 * i0 + 3305];
  }
  y[3307] += A[8282] * x[3306];
  y[3307] += A[8283] * x[3308];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3308] += A[1 * i0 + 8284] * x[1 * i0 + 3307];
  }
  y[3309] += A[8287] * x[3308];
  y[3309] += A[8288] * x[3310];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3310] += A[1 * i0 + 8289] * x[1 * i0 + 3309];
  }
  y[3311] += A[8292] * x[3310];
  y[3311] += A[8293] * x[3312];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3312] += A[1 * i0 + 8294] * x[1 * i0 + 3311];
  }
  y[3313] += A[8297] * x[3312];
  y[3313] += A[8298] * x[3314];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3314] += A[1 * i0 + 8299] * x[1 * i0 + 3313];
  }
  y[3315] += A[8302] * x[3314];
  y[3315] += A[8303] * x[3316];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3316] += A[1 * i0 + 8304] * x[1 * i0 + 3315];
  }
  y[3317] += A[8307] * x[3316];
  y[3317] += A[8308] * x[3318];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3318] += A[1 * i0 + 8309] * x[1 * i0 + 3317];
  }
  y[3319] += A[8312] * x[3318];
  y[3319] += A[8313] * x[3320];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3320] += A[1 * i0 + 8314] * x[1 * i0 + 3319];
  }
  y[3321] += A[8317] * x[3320];
  y[3321] += A[8318] * x[3322];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3322] += A[1 * i0 + 8319] * x[1 * i0 + 3321];
  }
  y[3323] += A[8322] * x[3322];
  y[3323] += A[8323] * x[3324];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3324] += A[1 * i0 + 8324] * x[1 * i0 + 3323];
  }
  y[3325] += A[8327] * x[3324];
  y[3325] += A[8328] * x[3326];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3326] += A[1 * i0 + 8329] * x[1 * i0 + 3325];
  }
  y[3327] += A[8332] * x[3326];
  y[3327] += A[8333] * x[3328];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3328] += A[1 * i0 + 8334] * x[1 * i0 + 3327];
  }
  y[3329] += A[8337] * x[3328];
  y[3329] += A[8338] * x[3330];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3330] += A[1 * i0 + 8339] * x[1 * i0 + 3329];
  }
  y[3331] += A[8342] * x[3330];
  y[3331] += A[8343] * x[3332];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3332] += A[1 * i0 + 8344] * x[1 * i0 + 3331];
  }
  y[3333] += A[8347] * x[3332];
  y[3333] += A[8348] * x[3334];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3334] += A[1 * i0 + 8349] * x[1 * i0 + 3333];
  }
  y[3335] += A[8352] * x[3334];
  y[3335] += A[8353] * x[3336];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3336] += A[1 * i0 + 8354] * x[1 * i0 + 3335];
  }
  y[3337] += A[8357] * x[3336];
  y[3337] += A[8358] * x[3338];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3338] += A[1 * i0 + 8359] * x[1 * i0 + 3337];
  }
  y[3339] += A[8362] * x[3338];
  y[3339] += A[8363] * x[3340];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3340] += A[1 * i0 + 8364] * x[1 * i0 + 3339];
  }
  y[3341] += A[8367] * x[3340];
  y[3341] += A[8368] * x[3342];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3342] += A[1 * i0 + 8369] * x[1 * i0 + 3341];
  }
  y[3343] += A[8372] * x[3342];
  y[3343] += A[8373] * x[3344];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3344] += A[1 * i0 + 8374] * x[1 * i0 + 3343];
  }
  y[3345] += A[8377] * x[3344];
  y[3345] += A[8378] * x[3346];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3346] += A[1 * i0 + 8379] * x[1 * i0 + 3345];
  }
  y[3347] += A[8382] * x[3346];
  y[3347] += A[8383] * x[3348];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3348] += A[1 * i0 + 8384] * x[1 * i0 + 3347];
  }
  y[3349] += A[8387] * x[3348];
  y[3349] += A[8388] * x[3350];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3350] += A[1 * i0 + 8389] * x[1 * i0 + 3349];
  }
  y[3351] += A[8392] * x[3350];
  y[3351] += A[8393] * x[3352];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3352] += A[1 * i0 + 8394] * x[1 * i0 + 3351];
  }
  y[3353] += A[8397] * x[3352];
  y[3353] += A[8398] * x[3354];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3354] += A[1 * i0 + 8399] * x[1 * i0 + 3353];
  }
  y[3355] += A[8402] * x[3354];
  y[3355] += A[8403] * x[3356];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3356] += A[1 * i0 + 8404] * x[1 * i0 + 3355];
  }
  y[3357] += A[8407] * x[3356];
  y[3357] += A[8408] * x[3358];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3358] += A[1 * i0 + 8409] * x[1 * i0 + 3357];
  }
  y[3359] += A[8412] * x[3358];
  y[3359] += A[8413] * x[3360];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3360] += A[1 * i0 + 8414] * x[1 * i0 + 3359];
  }
  y[3361] += A[8417] * x[3360];
  y[3361] += A[8418] * x[3362];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3362] += A[1 * i0 + 8419] * x[1 * i0 + 3361];
  }
  y[3363] += A[8422] * x[3362];
  y[3363] += A[8423] * x[3364];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3364] += A[1 * i0 + 8424] * x[1 * i0 + 3363];
  }
  y[3365] += A[8427] * x[3364];
  y[3365] += A[8428] * x[3366];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3366] += A[1 * i0 + 8429] * x[1 * i0 + 3365];
  }
  y[3367] += A[8432] * x[3366];
  y[3367] += A[8433] * x[3368];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3368] += A[1 * i0 + 8434] * x[1 * i0 + 3367];
  }
  y[3369] += A[8437] * x[3368];
  y[3369] += A[8438] * x[3370];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3370] += A[1 * i0 + 8439] * x[1 * i0 + 3369];
  }
  y[3371] += A[8442] * x[3370];
  y[3371] += A[8443] * x[3372];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 3372] += A[1 * i0 + 8444] * x[1 * i0 + 3371];
  }
  y[3372] += A[8446] * x[3533];
  y[3373] += A[8447] * x[5];
  y[3373] += A[8448] * x[12];
  y[3373] += A[8449] * x[3374];
  y[3373] += A[8450] * x[4026];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3374] += A[1 * i0 + 8451] * x[1 * i0 + 3373];
  }
  y[3375] += A[8454] * x[3374];
  y[3375] += A[8455] * x[3376];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3376] += A[1 * i0 + 8456] * x[1 * i0 + 3375];
  }
  y[3377] += A[8459] * x[3376];
  y[3377] += A[8460] * x[3378];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3378] += A[1 * i0 + 8461] * x[1 * i0 + 3377];
  }
  y[3379] += A[8464] * x[3378];
  y[3379] += A[8465] * x[3380];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3380] += A[1 * i0 + 8466] * x[1 * i0 + 3379];
  }
  y[3381] += A[8469] * x[3380];
  y[3381] += A[8470] * x[3382];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3382] += A[1 * i0 + 8471] * x[1 * i0 + 3381];
  }
  y[3383] += A[8474] * x[3382];
  y[3383] += A[8475] * x[3384];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3384] += A[1 * i0 + 8476] * x[1 * i0 + 3383];
  }
  y[3385] += A[8479] * x[3384];
  y[3385] += A[8480] * x[3386];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3386] += A[1 * i0 + 8481] * x[1 * i0 + 3385];
  }
  y[3387] += A[8484] * x[3386];
  y[3387] += A[8485] * x[3388];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3388] += A[1 * i0 + 8486] * x[1 * i0 + 3387];
  }
  y[3389] += A[8489] * x[3388];
  y[3389] += A[8490] * x[3390];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3390] += A[1 * i0 + 8491] * x[1 * i0 + 3389];
  }
  y[3391] += A[8494] * x[3390];
  y[3391] += A[8495] * x[3392];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3392] += A[1 * i0 + 8496] * x[1 * i0 + 3391];
  }
  y[3393] += A[8499] * x[3392];
  y[3393] += A[8500] * x[3394];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3394] += A[1 * i0 + 8501] * x[1 * i0 + 3393];
  }
  y[3395] += A[8504] * x[3394];
  y[3395] += A[8505] * x[3396];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3396] += A[1 * i0 + 8506] * x[1 * i0 + 3395];
  }
  y[3397] += A[8509] * x[3396];
  y[3397] += A[8510] * x[3398];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3398] += A[1 * i0 + 8511] * x[1 * i0 + 3397];
  }
  y[3399] += A[8514] * x[3398];
  y[3399] += A[8515] * x[3400];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3400] += A[1 * i0 + 8516] * x[1 * i0 + 3399];
  }
  y[3401] += A[8519] * x[3400];
  y[3401] += A[8520] * x[3402];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3402] += A[1 * i0 + 8521] * x[1 * i0 + 3401];
  }
  y[3403] += A[8524] * x[3402];
  y[3403] += A[8525] * x[3404];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3404] += A[1 * i0 + 8526] * x[1 * i0 + 3403];
  }
  y[3405] += A[8529] * x[3404];
  y[3405] += A[8530] * x[3406];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3406] += A[1 * i0 + 8531] * x[1 * i0 + 3405];
  }
  y[3407] += A[8534] * x[3406];
  y[3407] += A[8535] * x[3408];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3408] += A[1 * i0 + 8536] * x[1 * i0 + 3407];
  }
  y[3409] += A[8539] * x[3408];
  y[3409] += A[8540] * x[3410];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3410] += A[1 * i0 + 8541] * x[1 * i0 + 3409];
  }
  y[3411] += A[8544] * x[3410];
  y[3411] += A[8545] * x[3412];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3412] += A[1 * i0 + 8546] * x[1 * i0 + 3411];
  }
  y[3413] += A[8549] * x[3412];
  y[3413] += A[8550] * x[3414];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3414] += A[1 * i0 + 8551] * x[1 * i0 + 3413];
  }
  y[3415] += A[8554] * x[3414];
  y[3415] += A[8555] * x[3416];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3416] += A[1 * i0 + 8556] * x[1 * i0 + 3415];
  }
  y[3417] += A[8559] * x[3416];
  y[3417] += A[8560] * x[3418];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3418] += A[1 * i0 + 8561] * x[1 * i0 + 3417];
  }
  y[3419] += A[8564] * x[3418];
  y[3419] += A[8565] * x[3420];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3420] += A[1 * i0 + 8566] * x[1 * i0 + 3419];
  }
  y[3421] += A[8569] * x[3420];
  y[3421] += A[8570] * x[3422];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3422] += A[1 * i0 + 8571] * x[1 * i0 + 3421];
  }
  y[3423] += A[8574] * x[3422];
  y[3423] += A[8575] * x[3424];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3424] += A[1 * i0 + 8576] * x[1 * i0 + 3423];
  }
  y[3425] += A[8579] * x[3424];
  y[3425] += A[8580] * x[3426];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3426] += A[1 * i0 + 8581] * x[1 * i0 + 3425];
  }
  y[3427] += A[8584] * x[3426];
  y[3427] += A[8585] * x[3428];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3428] += A[1 * i0 + 8586] * x[1 * i0 + 3427];
  }
  y[3429] += A[8589] * x[3428];
  y[3429] += A[8590] * x[3430];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3430] += A[1 * i0 + 8591] * x[1 * i0 + 3429];
  }
  y[3431] += A[8594] * x[3430];
  y[3431] += A[8595] * x[3432];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3432] += A[1 * i0 + 8596] * x[1 * i0 + 3431];
  }
  y[3433] += A[8599] * x[3432];
  y[3433] += A[8600] * x[3434];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3434] += A[1 * i0 + 8601] * x[1 * i0 + 3433];
  }
  y[3435] += A[8604] * x[3434];
  y[3435] += A[8605] * x[3436];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3436] += A[1 * i0 + 8606] * x[1 * i0 + 3435];
  }
  y[3437] += A[8609] * x[3436];
  y[3437] += A[8610] * x[3438];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3438] += A[1 * i0 + 8611] * x[1 * i0 + 3437];
  }
  y[3439] += A[8614] * x[3438];
  y[3439] += A[8615] * x[3440];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3440] += A[1 * i0 + 8616] * x[1 * i0 + 3439];
  }
  y[3441] += A[8619] * x[3440];
  y[3441] += A[8620] * x[3442];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3442] += A[1 * i0 + 8621] * x[1 * i0 + 3441];
  }
  y[3443] += A[8624] * x[3442];
  y[3443] += A[8625] * x[3444];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3444] += A[1 * i0 + 8626] * x[1 * i0 + 3443];
  }
  y[3445] += A[8629] * x[3444];
  y[3445] += A[8630] * x[3446];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3446] += A[1 * i0 + 8631] * x[1 * i0 + 3445];
  }
  y[3447] += A[8634] * x[3446];
  y[3447] += A[8635] * x[3448];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3448] += A[1 * i0 + 8636] * x[1 * i0 + 3447];
  }
  y[3449] += A[8639] * x[3448];
  y[3449] += A[8640] * x[3450];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3450] += A[1 * i0 + 8641] * x[1 * i0 + 3449];
  }
  y[3451] += A[8644] * x[3450];
  y[3451] += A[8645] * x[3452];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3452] += A[1 * i0 + 8646] * x[1 * i0 + 3451];
  }
  y[3453] += A[8649] * x[3452];
  y[3453] += A[8650] * x[3454];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3454] += A[1 * i0 + 8651] * x[1 * i0 + 3453];
  }
  y[3455] += A[8654] * x[3454];
  y[3455] += A[8655] * x[3456];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3456] += A[1 * i0 + 8656] * x[1 * i0 + 3455];
  }
  y[3457] += A[8659] * x[3456];
  y[3457] += A[8660] * x[3458];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3458] += A[1 * i0 + 8661] * x[1 * i0 + 3457];
  }
  y[3459] += A[8664] * x[3458];
  y[3459] += A[8665] * x[3460];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3460] += A[1 * i0 + 8666] * x[1 * i0 + 3459];
  }
  y[3461] += A[8669] * x[3460];
  y[3461] += A[8670] * x[3462];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3462] += A[1 * i0 + 8671] * x[1 * i0 + 3461];
  }
  y[3463] += A[8674] * x[3462];
  y[3463] += A[8675] * x[3464];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3464] += A[1 * i0 + 8676] * x[1 * i0 + 3463];
  }
  y[3465] += A[8679] * x[3464];
  y[3465] += A[8680] * x[3466];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3466] += A[1 * i0 + 8681] * x[1 * i0 + 3465];
  }
  y[3467] += A[8684] * x[3466];
  y[3467] += A[8685] * x[3468];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3468] += A[1 * i0 + 8686] * x[1 * i0 + 3467];
  }
  y[3469] += A[8689] * x[3468];
  y[3469] += A[8690] * x[3470];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3470] += A[1 * i0 + 8691] * x[1 * i0 + 3469];
  }
  y[3471] += A[8694] * x[3470];
  y[3471] += A[8695] * x[3472];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3472] += A[1 * i0 + 8696] * x[1 * i0 + 3471];
  }
  y[3473] += A[8699] * x[3472];
  y[3473] += A[8700] * x[3474];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3474] += A[1 * i0 + 8701] * x[1 * i0 + 3473];
  }
  y[3475] += A[8704] * x[3474];
  y[3475] += A[8705] * x[3476];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3476] += A[1 * i0 + 8706] * x[1 * i0 + 3475];
  }
  y[3477] += A[8709] * x[3476];
  y[3477] += A[8710] * x[3478];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3478] += A[1 * i0 + 8711] * x[1 * i0 + 3477];
  }
  y[3479] += A[8714] * x[3478];
  y[3479] += A[8715] * x[3480];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3480] += A[1 * i0 + 8716] * x[1 * i0 + 3479];
  }
  y[3481] += A[8719] * x[3480];
  y[3481] += A[8720] * x[3482];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3482] += A[1 * i0 + 8721] * x[1 * i0 + 3481];
  }
  y[3483] += A[8724] * x[3482];
  y[3483] += A[8725] * x[3484];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3484] += A[1 * i0 + 8726] * x[1 * i0 + 3483];
  }
  y[3485] += A[8729] * x[3484];
  y[3485] += A[8730] * x[3486];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3486] += A[1 * i0 + 8731] * x[1 * i0 + 3485];
  }
  y[3487] += A[8734] * x[3486];
  y[3487] += A[8735] * x[3488];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3488] += A[1 * i0 + 8736] * x[1 * i0 + 3487];
  }
  y[3489] += A[8739] * x[3488];
  y[3489] += A[8740] * x[3490];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3490] += A[1 * i0 + 8741] * x[1 * i0 + 3489];
  }
  y[3491] += A[8744] * x[3490];
  y[3491] += A[8745] * x[3492];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3492] += A[1 * i0 + 8746] * x[1 * i0 + 3491];
  }
  y[3493] += A[8749] * x[3492];
  y[3493] += A[8750] * x[3494];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3494] += A[1 * i0 + 8751] * x[1 * i0 + 3493];
  }
  y[3495] += A[8754] * x[3494];
  y[3495] += A[8755] * x[3496];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3496] += A[1 * i0 + 8756] * x[1 * i0 + 3495];
  }
  y[3497] += A[8759] * x[3496];
  y[3497] += A[8760] * x[3498];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3498] += A[1 * i0 + 8761] * x[1 * i0 + 3497];
  }
  y[3499] += A[8764] * x[3498];
  y[3499] += A[8765] * x[3500];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3500] += A[1 * i0 + 8766] * x[1 * i0 + 3499];
  }
  y[3501] += A[8769] * x[3500];
  y[3501] += A[8770] * x[3502];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3502] += A[1 * i0 + 8771] * x[1 * i0 + 3501];
  }
  y[3503] += A[8774] * x[3502];
  y[3503] += A[8775] * x[3504];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3504] += A[1 * i0 + 8776] * x[1 * i0 + 3503];
  }
  y[3505] += A[8779] * x[3504];
  y[3505] += A[8780] * x[3506];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3506] += A[1 * i0 + 8781] * x[1 * i0 + 3505];
  }
  y[3507] += A[8784] * x[3506];
  y[3507] += A[8785] * x[3508];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3508] += A[1 * i0 + 8786] * x[1 * i0 + 3507];
  }
  y[3509] += A[8789] * x[3508];
  y[3509] += A[8790] * x[3510];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3510] += A[1 * i0 + 8791] * x[1 * i0 + 3509];
  }
  y[3511] += A[8794] * x[3510];
  y[3511] += A[8795] * x[3512];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3512] += A[1 * i0 + 8796] * x[1 * i0 + 3511];
  }
  y[3513] += A[8799] * x[3512];
  y[3513] += A[8800] * x[3514];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3514] += A[1 * i0 + 8801] * x[1 * i0 + 3513];
  }
  y[3515] += A[8804] * x[3514];
  y[3515] += A[8805] * x[3516];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3516] += A[1 * i0 + 8806] * x[1 * i0 + 3515];
  }
  y[3517] += A[8809] * x[3516];
  y[3517] += A[8810] * x[3518];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3518] += A[1 * i0 + 8811] * x[1 * i0 + 3517];
  }
  y[3519] += A[8814] * x[3518];
  y[3519] += A[8815] * x[3520];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3520] += A[1 * i0 + 8816] * x[1 * i0 + 3519];
  }
  y[3521] += A[8819] * x[3520];
  y[3521] += A[8820] * x[3522];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3522] += A[1 * i0 + 8821] * x[1 * i0 + 3521];
  }
  y[3523] += A[8824] * x[3522];
  y[3523] += A[8825] * x[3524];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3524] += A[1 * i0 + 8826] * x[1 * i0 + 3523];
  }
  y[3525] += A[8829] * x[3524];
  y[3525] += A[8830] * x[3526];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3526] += A[1 * i0 + 8831] * x[1 * i0 + 3525];
  }
  y[3527] += A[8834] * x[3526];
  y[3527] += A[8835] * x[3528];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3528] += A[1 * i0 + 8836] * x[1 * i0 + 3527];
  }
  y[3529] += A[8839] * x[3528];
  y[3529] += A[8840] * x[3530];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3530] += A[1 * i0 + 8841] * x[1 * i0 + 3529];
  }
  y[3531] += A[8844] * x[3530];
  y[3531] += A[8845] * x[3532];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3532] += A[1 * i0 + 8846] * x[1 * i0 + 3531];
  }
  y[3533] += A[8849] * x[11];
  y[3533] += A[8850] * x[3372];
  y[3533] += A[8851] * x[3532];
  y[3533] += A[8852] * x[4025];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 3534] += A[1 * i0 + 8853] * x[1 * i0 + 3534];
  }
  y[3535] += A[8855] * x[3534];
  y[3535] += A[8856] * x[3536];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3536] += A[1 * i0 + 8857] * x[1 * i0 + 3535];
  }
  y[3537] += A[8860] * x[3536];
  y[3537] += A[8861] * x[3538];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3538] += A[1 * i0 + 8862] * x[1 * i0 + 3537];
  }
}

void kernel_spmv_fragment_7(double *restrict A, double *restrict x,
                            double *restrict y) {
  register int i0, i1, i2, i3, i4, i5, i6;

  y[3539] += A[8865] * x[3538];
  y[3539] += A[8866] * x[3540];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3540] += A[1 * i0 + 8867] * x[1 * i0 + 3539];
  }
  y[3541] += A[8870] * x[3540];
  y[3541] += A[8871] * x[3542];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3542] += A[1 * i0 + 8872] * x[1 * i0 + 3541];
  }
  y[3543] += A[8875] * x[3542];
  y[3543] += A[8876] * x[3544];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3544] += A[1 * i0 + 8877] * x[1 * i0 + 3543];
  }
  y[3545] += A[8880] * x[3544];
  y[3545] += A[8881] * x[3546];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3546] += A[1 * i0 + 8882] * x[1 * i0 + 3545];
  }
  y[3547] += A[8885] * x[3546];
  y[3547] += A[8886] * x[3548];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3548] += A[1 * i0 + 8887] * x[1 * i0 + 3547];
  }
  y[3549] += A[8890] * x[3548];
  y[3549] += A[8891] * x[3550];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3550] += A[1 * i0 + 8892] * x[1 * i0 + 3549];
  }
  y[3551] += A[8895] * x[3550];
  y[3551] += A[8896] * x[3552];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3552] += A[1 * i0 + 8897] * x[1 * i0 + 3551];
  }
  y[3553] += A[8900] * x[3552];
  y[3553] += A[8901] * x[3554];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3554] += A[1 * i0 + 8902] * x[1 * i0 + 3553];
  }
  y[3555] += A[8905] * x[3554];
  y[3555] += A[8906] * x[3556];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3556] += A[1 * i0 + 8907] * x[1 * i0 + 3555];
  }
  y[3557] += A[8910] * x[3556];
  y[3557] += A[8911] * x[3558];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3558] += A[1 * i0 + 8912] * x[1 * i0 + 3557];
  }
  y[3559] += A[8915] * x[3558];
  y[3559] += A[8916] * x[3560];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3560] += A[1 * i0 + 8917] * x[1 * i0 + 3559];
  }
  y[3561] += A[8920] * x[3560];
  y[3561] += A[8921] * x[3562];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3562] += A[1 * i0 + 8922] * x[1 * i0 + 3561];
  }
  y[3563] += A[8925] * x[3562];
  y[3563] += A[8926] * x[3564];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3564] += A[1 * i0 + 8927] * x[1 * i0 + 3563];
  }
  y[3565] += A[8930] * x[3564];
  y[3565] += A[8931] * x[3566];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3566] += A[1 * i0 + 8932] * x[1 * i0 + 3565];
  }
  y[3567] += A[8935] * x[3566];
  y[3567] += A[8936] * x[3568];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3568] += A[1 * i0 + 8937] * x[1 * i0 + 3567];
  }
  y[3569] += A[8940] * x[3568];
  y[3569] += A[8941] * x[3570];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3570] += A[1 * i0 + 8942] * x[1 * i0 + 3569];
  }
  y[3571] += A[8945] * x[3570];
  y[3571] += A[8946] * x[3572];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3572] += A[1 * i0 + 8947] * x[1 * i0 + 3571];
  }
  y[3573] += A[8950] * x[3572];
  y[3573] += A[8951] * x[3574];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3574] += A[1 * i0 + 8952] * x[1 * i0 + 3573];
  }
  y[3575] += A[8955] * x[3574];
  y[3575] += A[8956] * x[3576];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3576] += A[1 * i0 + 8957] * x[1 * i0 + 3575];
  }
  y[3577] += A[8960] * x[3576];
  y[3577] += A[8961] * x[3578];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3578] += A[1 * i0 + 8962] * x[1 * i0 + 3577];
  }
  y[3579] += A[8965] * x[3578];
  y[3579] += A[8966] * x[3580];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3580] += A[1 * i0 + 8967] * x[1 * i0 + 3579];
  }
  y[3581] += A[8970] * x[3580];
  y[3581] += A[8971] * x[3582];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3582] += A[1 * i0 + 8972] * x[1 * i0 + 3581];
  }
  y[3583] += A[8975] * x[3582];
  y[3583] += A[8976] * x[3584];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3584] += A[1 * i0 + 8977] * x[1 * i0 + 3583];
  }
  y[3585] += A[8980] * x[3584];
  y[3585] += A[8981] * x[3586];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3586] += A[1 * i0 + 8982] * x[1 * i0 + 3585];
  }
  y[3587] += A[8985] * x[3586];
  y[3587] += A[8986] * x[3588];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3588] += A[1 * i0 + 8987] * x[1 * i0 + 3587];
  }
  y[3589] += A[8990] * x[3588];
  y[3589] += A[8991] * x[3590];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3590] += A[1 * i0 + 8992] * x[1 * i0 + 3589];
  }
  y[3591] += A[8995] * x[3590];
  y[3591] += A[8996] * x[3592];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3592] += A[1 * i0 + 8997] * x[1 * i0 + 3591];
  }
  y[3593] += A[9000] * x[3592];
  y[3593] += A[9001] * x[3594];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3594] += A[1 * i0 + 9002] * x[1 * i0 + 3593];
  }
  y[3595] += A[9005] * x[3594];
  y[3595] += A[9006] * x[3596];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3596] += A[1 * i0 + 9007] * x[1 * i0 + 3595];
  }
  y[3597] += A[9010] * x[3596];
  y[3597] += A[9011] * x[3598];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3598] += A[1 * i0 + 9012] * x[1 * i0 + 3597];
  }
  y[3599] += A[9015] * x[3598];
  y[3599] += A[9016] * x[3600];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3600] += A[1 * i0 + 9017] * x[1 * i0 + 3599];
  }
  y[3601] += A[9020] * x[3600];
  y[3601] += A[9021] * x[3602];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3602] += A[1 * i0 + 9022] * x[1 * i0 + 3601];
  }
  y[3603] += A[9025] * x[3602];
  y[3603] += A[9026] * x[3604];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3604] += A[1 * i0 + 9027] * x[1 * i0 + 3603];
  }
  y[3605] += A[9030] * x[3604];
  y[3605] += A[9031] * x[3606];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3606] += A[1 * i0 + 9032] * x[1 * i0 + 3605];
  }
  y[3607] += A[9035] * x[3606];
  y[3607] += A[9036] * x[3608];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3608] += A[1 * i0 + 9037] * x[1 * i0 + 3607];
  }
  y[3609] += A[9040] * x[3608];
  y[3609] += A[9041] * x[3610];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3610] += A[1 * i0 + 9042] * x[1 * i0 + 3609];
  }
  y[3611] += A[9045] * x[3610];
  y[3611] += A[9046] * x[3612];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3612] += A[1 * i0 + 9047] * x[1 * i0 + 3611];
  }
  y[3613] += A[9050] * x[3612];
  y[3613] += A[9051] * x[3614];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3614] += A[1 * i0 + 9052] * x[1 * i0 + 3613];
  }
  y[3615] += A[9055] * x[3614];
  y[3615] += A[9056] * x[3616];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3616] += A[1 * i0 + 9057] * x[1 * i0 + 3615];
  }
  y[3617] += A[9060] * x[3616];
  y[3617] += A[9061] * x[3618];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3618] += A[1 * i0 + 9062] * x[1 * i0 + 3617];
  }
  y[3619] += A[9065] * x[3618];
  y[3619] += A[9066] * x[3620];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3620] += A[1 * i0 + 9067] * x[1 * i0 + 3619];
  }
  y[3621] += A[9070] * x[3620];
  y[3621] += A[9071] * x[3622];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3622] += A[1 * i0 + 9072] * x[1 * i0 + 3621];
  }
  y[3623] += A[9075] * x[3622];
  y[3623] += A[9076] * x[3624];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3624] += A[1 * i0 + 9077] * x[1 * i0 + 3623];
  }
  y[3625] += A[9080] * x[3624];
  y[3625] += A[9081] * x[3626];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3626] += A[1 * i0 + 9082] * x[1 * i0 + 3625];
  }
  y[3627] += A[9085] * x[3626];
  y[3627] += A[9086] * x[3628];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3628] += A[1 * i0 + 9087] * x[1 * i0 + 3627];
  }
  y[3629] += A[9090] * x[3628];
  y[3629] += A[9091] * x[3630];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3630] += A[1 * i0 + 9092] * x[1 * i0 + 3629];
  }
  y[3631] += A[9095] * x[3630];
  y[3631] += A[9096] * x[3632];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3632] += A[1 * i0 + 9097] * x[1 * i0 + 3631];
  }
  y[3633] += A[9100] * x[3632];
  y[3633] += A[9101] * x[3634];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3634] += A[1 * i0 + 9102] * x[1 * i0 + 3633];
  }
  y[3635] += A[9105] * x[3634];
  y[3635] += A[9106] * x[3636];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3636] += A[1 * i0 + 9107] * x[1 * i0 + 3635];
  }
  y[3637] += A[9110] * x[3636];
  y[3637] += A[9111] * x[3638];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3638] += A[1 * i0 + 9112] * x[1 * i0 + 3637];
  }
  y[3639] += A[9115] * x[3638];
  y[3639] += A[9116] * x[3640];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3640] += A[1 * i0 + 9117] * x[1 * i0 + 3639];
  }
  y[3641] += A[9120] * x[3640];
  y[3641] += A[9121] * x[3642];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3642] += A[1 * i0 + 9122] * x[1 * i0 + 3641];
  }
  y[3643] += A[9125] * x[3642];
  y[3643] += A[9126] * x[3644];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3644] += A[1 * i0 + 9127] * x[1 * i0 + 3643];
  }
  y[3645] += A[9130] * x[3644];
  y[3645] += A[9131] * x[3646];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3646] += A[1 * i0 + 9132] * x[1 * i0 + 3645];
  }
  y[3647] += A[9135] * x[3646];
  y[3647] += A[9136] * x[3648];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3648] += A[1 * i0 + 9137] * x[1 * i0 + 3647];
  }
  y[3649] += A[9140] * x[3648];
  y[3649] += A[9141] * x[3650];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3650] += A[1 * i0 + 9142] * x[1 * i0 + 3649];
  }
  y[3651] += A[9145] * x[3650];
  y[3651] += A[9146] * x[3652];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3652] += A[1 * i0 + 9147] * x[1 * i0 + 3651];
  }
  y[3653] += A[9150] * x[3652];
  y[3653] += A[9151] * x[3654];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3654] += A[1 * i0 + 9152] * x[1 * i0 + 3653];
  }
  y[3655] += A[9155] * x[3654];
  y[3655] += A[9156] * x[3656];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3656] += A[1 * i0 + 9157] * x[1 * i0 + 3655];
  }
  y[3657] += A[9160] * x[3656];
  y[3657] += A[9161] * x[3658];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3658] += A[1 * i0 + 9162] * x[1 * i0 + 3657];
  }
  y[3659] += A[9165] * x[3658];
  y[3659] += A[9166] * x[3660];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3660] += A[1 * i0 + 9167] * x[1 * i0 + 3659];
  }
  y[3661] += A[9170] * x[3660];
  y[3661] += A[9171] * x[3662];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3662] += A[1 * i0 + 9172] * x[1 * i0 + 3661];
  }
  y[3663] += A[9175] * x[3662];
  y[3663] += A[9176] * x[3664];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3664] += A[1 * i0 + 9177] * x[1 * i0 + 3663];
  }
  y[3665] += A[9180] * x[3664];
  y[3665] += A[9181] * x[3666];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3666] += A[1 * i0 + 9182] * x[1 * i0 + 3665];
  }
  y[3667] += A[9185] * x[3666];
  y[3667] += A[9186] * x[3668];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3668] += A[1 * i0 + 9187] * x[1 * i0 + 3667];
  }
  y[3669] += A[9190] * x[3668];
  y[3669] += A[9191] * x[3670];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3670] += A[1 * i0 + 9192] * x[1 * i0 + 3669];
  }
  y[3671] += A[9195] * x[3670];
  y[3671] += A[9196] * x[3672];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3672] += A[1 * i0 + 9197] * x[1 * i0 + 3671];
  }
  y[3673] += A[9200] * x[3672];
  y[3673] += A[9201] * x[3674];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3674] += A[1 * i0 + 9202] * x[1 * i0 + 3673];
  }
  y[3675] += A[9205] * x[3674];
  y[3675] += A[9206] * x[3676];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3676] += A[1 * i0 + 9207] * x[1 * i0 + 3675];
  }
  y[3677] += A[9210] * x[3676];
  y[3677] += A[9211] * x[3678];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3678] += A[1 * i0 + 9212] * x[1 * i0 + 3677];
  }
  y[3679] += A[9215] * x[3678];
  y[3679] += A[9216] * x[3680];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3680] += A[1 * i0 + 9217] * x[1 * i0 + 3679];
  }
  y[3681] += A[9220] * x[3680];
  y[3681] += A[9221] * x[3682];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3682] += A[1 * i0 + 9222] * x[1 * i0 + 3681];
  }
  y[3683] += A[9225] * x[3682];
  y[3683] += A[9226] * x[3684];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3684] += A[1 * i0 + 9227] * x[1 * i0 + 3683];
  }
  y[3685] += A[9230] * x[3684];
  y[3685] += A[9231] * x[3686];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3686] += A[1 * i0 + 9232] * x[1 * i0 + 3685];
  }
  y[3687] += A[9235] * x[3686];
  y[3687] += A[9236] * x[3688];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3688] += A[1 * i0 + 9237] * x[1 * i0 + 3687];
  }
  y[3689] += A[9240] * x[3688];
  y[3689] += A[9241] * x[3690];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[0 * i0 + 3690] += A[1 * i0 + 9242] * x[1 * i0 + 3689];
  }
  y[3691] += A[9245] * x[3690];
  y[3691] += A[9246] * x[3692];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 3692] += A[1 * i0 + 9247] * x[1 * i0 + 3691];
  }
  y[3692] += A[9249] * x[4012];
  y[3693] += A[9250] * x[3694];
  y[3693] += A[9251] * x[3852];
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3694] += A[2 * i0 + 9252] * x[1 * i0 + 3693];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3694] += A[2 * i0 + 9253] * x[1 * i0 + 3695];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3702] += A[2 * i0 + 9268] * x[1 * i0 + 3701];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3702] += A[2 * i0 + 9269] * x[1 * i0 + 3703];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3710] += A[2 * i0 + 9284] * x[1 * i0 + 3709];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3710] += A[2 * i0 + 9285] * x[1 * i0 + 3711];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3718] += A[2 * i0 + 9300] * x[1 * i0 + 3717];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3718] += A[2 * i0 + 9301] * x[1 * i0 + 3719];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3726] += A[2 * i0 + 9316] * x[1 * i0 + 3725];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3726] += A[2 * i0 + 9317] * x[1 * i0 + 3727];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3734] += A[2 * i0 + 9332] * x[1 * i0 + 3733];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3734] += A[2 * i0 + 9333] * x[1 * i0 + 3735];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3742] += A[2 * i0 + 9348] * x[1 * i0 + 3741];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3742] += A[2 * i0 + 9349] * x[1 * i0 + 3743];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3750] += A[2 * i0 + 9364] * x[1 * i0 + 3749];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3750] += A[2 * i0 + 9365] * x[1 * i0 + 3751];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3758] += A[2 * i0 + 9380] * x[1 * i0 + 3757];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3758] += A[2 * i0 + 9381] * x[1 * i0 + 3759];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3766] += A[2 * i0 + 9396] * x[1 * i0 + 3765];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3766] += A[2 * i0 + 9397] * x[1 * i0 + 3767];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3774] += A[2 * i0 + 9412] * x[1 * i0 + 3773];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3774] += A[2 * i0 + 9413] * x[1 * i0 + 3775];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3782] += A[2 * i0 + 9428] * x[1 * i0 + 3781];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3782] += A[2 * i0 + 9429] * x[1 * i0 + 3783];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3790] += A[2 * i0 + 9444] * x[1 * i0 + 3789];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3790] += A[2 * i0 + 9445] * x[1 * i0 + 3791];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3798] += A[2 * i0 + 9460] * x[1 * i0 + 3797];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3798] += A[2 * i0 + 9461] * x[1 * i0 + 3799];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3806] += A[2 * i0 + 9476] * x[1 * i0 + 3805];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3806] += A[2 * i0 + 9477] * x[1 * i0 + 3807];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3814] += A[2 * i0 + 9492] * x[1 * i0 + 3813];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3814] += A[2 * i0 + 9493] * x[1 * i0 + 3815];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3822] += A[2 * i0 + 9508] * x[1 * i0 + 3821];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3822] += A[2 * i0 + 9509] * x[1 * i0 + 3823];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3830] += A[2 * i0 + 9524] * x[1 * i0 + 3829];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3830] += A[2 * i0 + 9525] * x[1 * i0 + 3831];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3838] += A[2 * i0 + 9540] * x[1 * i0 + 3837];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3838] += A[2 * i0 + 9541] * x[1 * i0 + 3839];
  }
  for (i0 = 0; i0 <= 5; ++i0) {
    y[1 * i0 + 3846] += A[2 * i0 + 9556] * x[1 * i0 + 3845];
  }
  for (i0 = 0; i0 <= 4; ++i0) {
    y[1 * i0 + 3846] += A[2 * i0 + 9557] * x[1 * i0 + 3847];
  }
  y[3851] += A[9567] * x[4012];
  y[3852] += A[9568] * x[3693];
  y[3852] += A[9569] * x[3853];
  y[3852] += A[9570] * x[4027];
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3853] += A[2 * i0 + 9571] * x[1 * i0 + 3852];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3853] += A[2 * i0 + 9572] * x[1 * i0 + 3854];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3861] += A[2 * i0 + 9587] * x[1 * i0 + 3860];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3861] += A[2 * i0 + 9588] * x[1 * i0 + 3862];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3869] += A[2 * i0 + 9603] * x[1 * i0 + 3868];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3869] += A[2 * i0 + 9604] * x[1 * i0 + 3870];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3877] += A[2 * i0 + 9619] * x[1 * i0 + 3876];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3877] += A[2 * i0 + 9620] * x[1 * i0 + 3878];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3885] += A[2 * i0 + 9635] * x[1 * i0 + 3884];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3885] += A[2 * i0 + 9636] * x[1 * i0 + 3886];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3893] += A[2 * i0 + 9651] * x[1 * i0 + 3892];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3893] += A[2 * i0 + 9652] * x[1 * i0 + 3894];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3901] += A[2 * i0 + 9667] * x[1 * i0 + 3900];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3901] += A[2 * i0 + 9668] * x[1 * i0 + 3902];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3909] += A[2 * i0 + 9683] * x[1 * i0 + 3908];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3909] += A[2 * i0 + 9684] * x[1 * i0 + 3910];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3917] += A[2 * i0 + 9699] * x[1 * i0 + 3916];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3917] += A[2 * i0 + 9700] * x[1 * i0 + 3918];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3925] += A[2 * i0 + 9715] * x[1 * i0 + 3924];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3925] += A[2 * i0 + 9716] * x[1 * i0 + 3926];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3933] += A[2 * i0 + 9731] * x[1 * i0 + 3932];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3933] += A[2 * i0 + 9732] * x[1 * i0 + 3934];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3941] += A[2 * i0 + 9747] * x[1 * i0 + 3940];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3941] += A[2 * i0 + 9748] * x[1 * i0 + 3942];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3949] += A[2 * i0 + 9763] * x[1 * i0 + 3948];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3949] += A[2 * i0 + 9764] * x[1 * i0 + 3950];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3957] += A[2 * i0 + 9779] * x[1 * i0 + 3956];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3957] += A[2 * i0 + 9780] * x[1 * i0 + 3958];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3965] += A[2 * i0 + 9795] * x[1 * i0 + 3964];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3965] += A[2 * i0 + 9796] * x[1 * i0 + 3966];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3973] += A[2 * i0 + 9811] * x[1 * i0 + 3972];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3973] += A[2 * i0 + 9812] * x[1 * i0 + 3974];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3981] += A[2 * i0 + 9827] * x[1 * i0 + 3980];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3981] += A[2 * i0 + 9828] * x[1 * i0 + 3982];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3989] += A[2 * i0 + 9843] * x[1 * i0 + 3988];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3989] += A[2 * i0 + 9844] * x[1 * i0 + 3990];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3997] += A[2 * i0 + 9859] * x[1 * i0 + 3996];
  }
  for (i0 = 0; i0 <= 7; ++i0) {
    y[1 * i0 + 3997] += A[2 * i0 + 9860] * x[1 * i0 + 3998];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 4005] += A[2 * i0 + 9875] * x[1 * i0 + 4004];
  }
  for (i0 = 0; i0 <= 6; ++i0) {
    y[1 * i0 + 4005] += A[2 * i0 + 9876] * x[1 * i0 + 4006];
  }
  y[4012] += A[9889] * x[3];
  y[4012] += A[9890] * x[1453];
  y[4012] += A[9891] * x[1771];
  y[4012] += A[9892] * x[2091];
  y[4012] += A[9893] * x[3692];
  y[4012] += A[9894] * x[3851];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 4012] += A[1 * i0 + 9895] * x[1 * i0 + 4011];
  }
  y[4013] += A[9897] * x[14];
  y[4013] += A[9898] * x[4013];
  for (i0 = 0; i0 <= 1; ++i0) {
    y[0 * i0 + 4014] += A[1 * i0 + 9899] * x[1 * i0 + 334];
  }
  y[4015] += A[9901] * x[174];
  y[4015] += A[9902] * x[1132];
  y[4015] += A[9903] * x[4015];
  y[4016] += A[9904] * x[1612];
  y[4016] += A[9905] * x[4016];
  y[4017] += A[9906] * x[813];
  y[4018] += A[9907] * x[1292];
  y[4019] += A[9908] * x[2411];
  y[4019] += A[9909] * x[2731];
  y[4020] += A[9910] * x[334];
  y[4020] += A[9911] * x[2732];
  y[4021] += A[9912] * x[1132];
  y[4021] += A[9913] * x[2892];
  y[4022] += A[9914] * x[1452];
  y[4022] += A[9915] * x[2893];
  for (i0 = 0; i0 <= 2; ++i0) {
    y[1 * i0 + 4022] += A[2 * i0 + 9916] * x[1 * i0 + 4022];
  }
  y[4023] += A[9917] * x[3212];
  y[4024] += A[9919] * x[3213];
  y[4025] += A[9921] * x[1932];
  y[4025] += A[9922] * x[3533];
  y[4025] += A[9923] * x[4025];
  y[4026] += A[9924] * x[3373];
  y[4026] += A[9925] * x[4026];
  y[4027] += A[9926] * x[3852];
}

void kernel_spmv(double *restrict A, double *restrict x, double *restrict y) {
  kernel_spmv_fragment_0(A, x, y);
  // kernel_spmv_fragment_1(A, x, y);
  // kernel_spmv_fragment_2(A, x, y);
  // kernel_spmv_fragment_3(A, x, y);
  // kernel_spmv_fragment_4(A, x, y);
  // kernel_spmv_fragment_5(A, x, y);
  // kernel_spmv_fragment_6(A, x, y);
  // kernel_spmv_fragment_7(A, x, y);
}
