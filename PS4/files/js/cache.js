function DLProgress(e){
	parent.ScrOverlay.style.display="block";
	Percent=(Math.round(e.loaded / e.total * 100));
	parent.cacheUPDtxt.innerHTML="Установка Оффлайн Кэша: " + Percent + "%.";
	parent.CacheBar.style.width=Percent + '%';
}
function DisplayCacheProgress(){
	setTimeout(function(){parent.cacheUPDtxt.innerHTML="Оффлайн Кэш установлен успешно!!! Перезагрузите Веб-Браузер.";},1000);}
	window.applicationCache.addEventListener("progress",DLProgress,false);
	window.applicationCache.oncached=function(e){DisplayCacheProgress();};
	window.applicationCache.onupdateready=function(e){DisplayCacheProgress();};
	window.applicationCache.onerror=function(e){parent.cacheUPDtxt.innerHTML="Оффлайн Режим";
};