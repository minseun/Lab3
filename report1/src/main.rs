use std::io;

fn main() {
    println!("행렬의 행과 열의 수를 입력하세요:");
    let (rows, cols) = {
        let mut input = String::new();
        io::stdin().read_line(&mut input).expect("입력을 읽지 못했습니다.");
        let dims: Vec<usize> = input.trim().split_whitespace()
            .map(|x| x.parse::<usize>().expect("유효한 숫자를 입력하세요"))
            .collect();

        if dims.len() != 2 {
            panic!("행과 열의 수를 정확히 두 개 입력하세요.");
        }

        (dims[0], dims[1])
    };

    println!("첫 번째 행렬의 요소를 입력하세요:");
    let matrix_a = read_matrix(rows, cols);

    println!("두 번째 행렬의 요소를 입력하세요:");
    let matrix_b = read_matrix(rows, cols);

    let result_matrix = add_matrices(&matrix_a, &matrix_b, rows, cols);

    println!("행렬 덧셈의 결과는 다음과 같습니다:");
    for i in 0..rows {
        for j in 0..cols {
            print!("{} ", result_matrix[i * cols + j]);
        }
        println!();
    }
}

fn read_matrix(rows: usize, cols: usize) -> Vec<i32> {
    let mut matrix = Vec::with_capacity(rows * cols);
    for i in 0..rows {
        let mut input = String::new();
        io::stdin().read_line(&mut input).expect("입력을 읽지 못했습니다.");
        let row_elements: Vec<i32> = input.trim()
                                          .split_whitespace()
                                          .map(|x| x.parse().expect("유효한 정수를 입력하세요"))
                                          .collect();

        if row_elements.len() != cols {
            panic!("{}번째 행은 정확히 {}개의 요소가 필요합니다.", i + 1, cols);
        }

        matrix.extend(row_elements);
    }
    matrix
}

fn add_matrices(matrix_a: &Vec<i32>, matrix_b: &Vec<i32>, rows: usize, cols: usize) -> Vec<i32> {
    let mut result = Vec::with_capacity(rows * cols);
    for i in 0..(rows * cols) {
        result.push(matrix_a[i] + matrix_b[i]);
    }
    result
}
