void stencil(float *input_vec, float *output_vec, float *filter_vec, int m, int n, int k, int b) {
    float (*input)[m][n] = (float (*)[m][n]) input_vec;
    float (*filter)[k] = (float (*)[k]) filter_vec;
    float (*output)[m][n] = (float (*)[m][n]) output_vec;

    int blower = (k - 1) / 2;
    int bupper = k / 2;
    
    #pragma omp parallel for collapse(3)
    for (int batch = 0; batch < b; ++batch) {
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                // If in boundary region, just copy input
                if (i < blower || i >= m - bupper || j < blower || j >= n - bupper) {
                    output[batch][i][j] = input[batch][i][j];
                    continue;
                }

                float sum = 0.0f;
                for (int u = 0; u < k; ++u) {
                    for (int v = 0; v < k; ++v) {
                        int row = i + u - blower;
                        int col = j + v - blower;
                        sum += input[batch][row][col] * filter[u][v];
                    }
                }
                output[batch][i][j] = sum;
            }
        }
    }
}
