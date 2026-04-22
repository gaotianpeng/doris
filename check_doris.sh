for ref in 4.0.4-release 4.0.5 4.1.0 master; do
  echo "--- REF: $ref ---"
  found_file=""
  for file in be/src/core/data_type_serde/data_type_string_serde.cpp be/src/vec/data_types/serde/data_type_string_serde.cpp; do
    if curl -s -f "https://raw.githubusercontent.com/apache/doris/$ref/$file" > /dev/null; then
      found_file=$file
      break
    fi
  done
  if [ -n "$found_file" ]; then
    echo "FILE: $found_file"
    curl -s "https://raw.githubusercontent.com/apache/doris/$ref/$found_file" | grep -A 15 -E 'deserialize_one_cell_from_(csv|hive_text)' | grep -E 'escape_string|slice.data'
  else
    echo "FILE: NOT FOUND"
  fi
  found_test=""
  for test_file in be/test/core/data_type_serde/data_type_serde_csv_test.cpp be/test/vec/data_types/serde/data_type_serde_csv_test.cpp; do
    if curl -s -f "https://raw.githubusercontent.com/apache/doris/$ref/$test_file" > /dev/null; then
      found_test=$test_file
      break
    fi
  done
  if [ -n "$found_test" ]; then
    echo "TEST: $found_test"
    curl -s "https://raw.githubusercontent.com/apache/doris/$ref/$found_test" | grep -E 'ComplexTypeSerdeSchemaChangedCsvTest|\\N'
  fi
done
