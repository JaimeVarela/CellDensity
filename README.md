# Densidad de células en una imagen de tejido
El programa se ha creado utilizando las librerías OpenCV. Su objetivo es contar la cantidad de células que se encuentran en una imagen de un tejido y su densidad, filtrando el estroma.

1. Se carga la imagen a analizar. Se empleará como ejemplo:

![tejido](https://user-images.githubusercontent.com/44776831/48086791-fcdb3500-e1fd-11e8-8898-0f19b7f9b447.png)

2. Se calcula los bordes de la imagen empleando la función openCV adaptiveThreshold, creando la siguiente imagen:

![adaptive](https://user-images.githubusercontent.com/44776831/48086933-4fb4ec80-e1fe-11e8-979a-669cbe616a76.jpg)

3. Después se emplea las funciones openCV findContours y drawContours para hallar los contornos de la imagen anterior:

![contours](https://user-images.githubusercontent.com/44776831/48087114-bb975500-e1fe-11e8-8716-5560e7da3b7c.jpg)

4. Las células tienen una forma circular, por lo que descartamos aquellos elementos de la imagen que no cumplan esa forma, de forma que se filtra el estroma:

![celulas](https://user-images.githubusercontent.com/44776831/48087198-fd280000-e1fe-11e8-9672-a3df44fece27.jpg)

5. Finalmente se crea el mapa de densidad:

![densidad](https://user-images.githubusercontent.com/44776831/48087286-319bbc00-e1ff-11e8-8deb-054fa5dc0072.png)
