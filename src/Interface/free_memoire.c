void liberer_memoire(int** tab1, int** tab2, int size121, int* tab3, int** tab4, int size341, int** tab5, int** tab6, SCell* tabcel) {
  int i;
  for (i = 0; i < size121; i++) {
      free(tab1[i]);
      free(tab2[i]);
  }
  free(tab1);
  free(tab2);

  for (i = 0; i < size341; i++) {
    free(tab4[i]);
  }
  free(tab3);
  free(tab4);

  for (i = 0; i < size341; i++) {
    free(tab5[i]);
    free(tab6[i]);
  }
  free(tab5);
  free(tab6);

  for (i = 0; i < size341; i++) {
    free(tabcel[i].neighbors[i]);
  }
  for (i = 0; i < size341; i++) {
    free(tabcel[i].neighbors);
  }
  free(tabcel);
}
