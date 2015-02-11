*Másolni nem ér!*
<h1>Grafika házi feladatok (2014/2015/1)</h1>

<h2>1. feladat</h2>

Készítsen "Síkon táncoló kontrollpontok" programot. A felhasználó az egér balgomb egyszeri lenyomással/elengedéssel veszi fel a kontrollpontokat (max 10-et), amelyekhez 2cm sugarú kisebb fekete köröket rendelünk. Ha a kontrollpontok száma legalább kettő, azokra egy türkiszkék színű, kitöltött konvex burkot, piros Bézier görbét, nulla kezdő és végsebességű, a kontrollpont lehelyezésének idejét paraméterként használó zöld Catmull-Rom spline-t és kék Catmull-Clark görbét illeszt.  
A háttér világosszürke. Legnagyobb prioritása a kontrollpontoknak van, majd a görbék jönnek, végül jön a konvex burok.
Space lenyomására a kontrollpontok egy-egy 5 cm-es kör tetejéről elindulva, 5 másodpercenként egy teljes fordulatot téve, elkezdenek keringeni, mégpedig a páros indexűek az óramutató járásával megegyező, a páratlan indexűek pedig azzal ellentétes irányban. A konvex burok és a görbék követik a kontrollpontokat. Mindezt a felhasználó egy 58cm x 68 cm-es kameraablakon keresztül látja. Ha a felhasználó az egér jobb gombbal rábök egy kontrollpontra, akkor a kameraablakot ehhez köti, a kontrollpont elmozdulása automatikusan a kameraablakot is arrébb viszi.  

<h2>2. feladat</h2>

Készítsen csendéletet, ahol procedurálisan textúrázott, diffúz+Blinn spekuláris asztalon optikailag sima tárgyak, egy ellipszoid aranykaktusz, egy paraboloid ezüstkaktusz és egy henger üvegkaktusz áll (a háromból min. kettő megvalósítása szükséges). A kaktuszoktörzséből a törzzsel hasonló, de kisebb részek nőnek ki, mindig a felületre merőlegesen, legalább két további szinten (az összes részek számát úgy kell megválasztani, hogy a program 1 percen belül lefusson). A kinövő részek véletlenszerűen, nagyjából egyenletesen oszlanak el a felületen. A teret egy zöldes, egy kékes, és egy pirosas pontfényforrás világítja meg, a megvilágítási intenzitás a távolság négyzetével csökken. Az égbolt világoskék. Árnyékok vannak. A kamera az asztal fölött van és enyhén lefelé néz.  
A sugár-objektum metszéspontot és a normálvektort a metszéspontban analitikusan kell számolni.  
Az említett anyagok törésmutatója (n) és kioltási tényezője (k):  
...................r..........g..........b  
Üveg (n/k)......1.5/0.0, 1.5/0.0, 1.5/0.0  
Arany (n/k).....0.17/3.1, 0.35/2.7, 1.5/1.9  
Ezüst (n/k).....0.14/4.1, 0.16/2.3, 0.13/3.1  

<h2>3-4. feladat</h2>

Készítsen szimulátort műhold befogáshoz a MIR űrállomás számára! A fényforrás a távoli nap. Az űrállomás a csillagos égbolt előtt, egy diffúz, procedurálisan textúrázott bolygó körül kering, pályája nem geostracionárius. A bolygót átlátszó kékes légkör veszi körül. Az űrállomás teste fémes csillogású forgástest, a sziluett Catmull-Rom spline-nal adandó meg. Az űrállomáson van egy nagyjából kör alakú lyuk. Az űrállomáshoz legalább két napelem tábla tartozik. Az űrállomás egy, a fő tehetetlenségi iránytól különböző tengely körül lassan forog.  
Az avatárunk éppen űrsétát végez, magát az űrállomáshoz kötve egy hosszú gumikötél végén rángatózva. A gumikötél csak egy adott távolság után feszül meg, azon túl Hooke törvény szerint nyúlik. Az űrállomástól nem zérus sebességgel rúgtuk el magunkat. Avatárunk tömege az űrállomás tömegénél sokkal kisebb. Az avatár mindig az űrállomás nyílása felé néz.  
A mesterséges hold gömb alakú, amiből legalább három henger vagy kúp alakú fúvóka nyúlik ki (bónusz: ha a fúvókák tengelye nem megy át a súlyponton, akkor a forgását is szabályozhatjuk). Kezdetben a műhold az űrállomáshoz képest lassan halad és forog. A műhold ugyancsak diffúz-spekuláris, a fúvókák színe alapján vezérelhetjük őket. Ha a q, w, ... billentyűket lenyomjuk, a betűhöz rendelt fúvókán egy lángnyelv tör ki, a műhold pedig ellentétes irányú I impulzust kap. A lendület és perdület, mint mindig, itt is megmarad.  
A játék célja, hogy a mesterséges hold a műhold nyílásán besétáljon.  
A harmadik feladat a játék "csendélete", még nem kell az objektumokat animálni és lángnyelvet sem kell kilőni. A virtuális világ életre keltése és vezérlése a negyedik házi feladata lesz.   

*Másolni nem ér!*

*kérdezni szabad*
