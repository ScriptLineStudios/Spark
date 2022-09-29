import unittest
import spark

spark.init("Key Test", 1, 1, False) 

class TestTexure(unittest.TestCase):
    def test_key_pressed(self):
        self.assertFalse(spark.key_is_pressed(65))
        self.assertFalse(spark.key_is_pressed(66))
        self.assertFalse(spark.key_is_pressed(67))
        self.assertFalse(spark.key_is_pressed(68))
        self.assertFalse(spark.key_is_pressed(69))
        self.assertFalse(spark.key_is_pressed(70))
        self.assertFalse(spark.key_is_pressed(71))


if __name__ == '__main__':
    unittest.main()