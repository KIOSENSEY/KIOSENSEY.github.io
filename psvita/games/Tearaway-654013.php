<?
include $_SERVER['DOCUMENT_ROOT']."/php/vars_ban.php";
$klin[1] = "aHR0cHM6Ly92ay5jb20vZG9jLTE5Njc2NjQzM182Njk3ODExMTY/aGFzaD1FNjVPdFRvT3ZHenhxUTh4aE9ZTnl1azBoVUlwR3pibzRFZkxjR1I1WVpzJmRs";
$klin[2] = "aHR0cHM6Ly92ay5jb20vZG9jLTE5Njc2NjQzM182Njk3ODExMjI/aGFzaD02R29uWmlVc3Zad2YyMDdaelB6UmNoQjJQSnZ6ZG82eEVrOUgxeTcwdXpYJmRs";
$klin[3] = "";
$klin[4] = "";
$klin[5] = "";
$klin[6] = "";
$klin[7] = "";
$klin[8] = "";
$klin[9] = "";
$klin[10] = "";
$klin[11] = "";
$klin[12] = "";
$klin[13] = "";
$klin[14] = "";
$klin[15] = "";

$milks[1] = base64_decode($klin[1]);
$milks[2] = base64_decode($klin[2]);
$milks[3] = base64_decode($klin[3]);
$milks[4] = base64_decode($klin[4]);
$milks[5] = base64_decode($klin[5]);
$milks[6] = base64_decode($klin[6]);
$milks[7] = base64_decode($klin[7]);
$milks[8] = base64_decode($klin[8]);
$milks[9] = base64_decode($klin[9]);
$milks[10] = base64_decode($klin[10]);
$milks[11] = base64_decode($klin[11]);
$milks[12] = base64_decode($klin[12]);
$milks[13] = base64_decode($klin[13]);
$milks[14] = base64_decode($klin[14]);
$milks[15] = base64_decode($klin[15]); 
if ($reclamma_catcut == 2){ $klin = $milks; $link_catcut = "";}

$links[1] = $link_ouo.$link_catcut.$klin[1];
$links[2] = "$link_catcut".$klin[2];
$links[3] = "$link_catcut".$klin[3];
$links[4] = "$link_catcut".$klin[4];
$links[5] = "$link_catcut".$klin[5];
$links[6] = "$link_catcut".$klin[6];
$links[7] = "$link_catcut".$klin[7];
$links[8] = "$link_catcut".$klin[8];
$links[9] = "$link_catcut".$klin[9];
$links[10] = "$link_catcut".$klin[10];
$links[11] = "$link_catcut".$klin[11];
$links[12] = "$link_catcut".$klin[12];
$links[13] = "$link_catcut".$klin[13];
$links[14] = "$link_catcut".$klin[14];
$links[15] = "$link_catcut".$klin[15];
if(isset($_GET['id']) && is_numeric($_GET['id']) && isset($links[$_GET['id']])){
	if (!empty($_SESSION['login'])){
		$link = $milks[$_GET['id']];
		}
	else {
		$link = $links[$_GET['id']];
		}
	header("Location: $link");
	}
?>