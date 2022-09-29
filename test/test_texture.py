import unittest
import spark

spark.init("Texture Test", 1, 1, False) 

img_path = "test-assets/player_walk2.png"

class TestTexure(unittest.TestCase):
    def test_texture_creation(self):
        tex0 = spark.create_rect(img_path)
        self.assertEqual(tex0, 0)

        tex1 = spark.create_rect(img_path)
        self.assertEqual(tex1, 1)

        tex2 = spark.create_rect(img_path)
        self.assertEqual(tex2, 2)

    def test_texture_file_error(self):
        invalid_paths = ["yes.png", "no.png", "ok.jpg", "examples/1.png", "test-assets/2.png"]

        for path in invalid_paths:
            self.assertRaises(FileNotFoundError, spark.create_rect, path)

if __name__ == '__main__':
    unittest.main()